# --- Check what formats is being stored in the current session's clipboard ---
# Quick plain text truncation of the clipboard, run following in powershell
# > Get-Clipboard -Format Text | Set-Clipboard

# --- Win32 P/Invoke: user32 ---

Add-Type -Namespace Win32 -Name User32 -MemberDefinition @"
[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError=true)]
public static extern bool OpenClipboard(System.IntPtr hWndNewOwner);

[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError=true)]
public static extern bool CloseClipboard();

[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError=true)]
public static extern uint EnumClipboardFormats(uint format);

[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError=true)]
public static extern int GetClipboardFormatName(uint format, System.Text.StringBuilder lpszFormatName, int cchMaxCount);

[System.Runtime.InteropServices.DllImport("user32.dll", SetLastError=true)]
public static extern System.IntPtr GetClipboardData(uint uFormat);

[System.Runtime.InteropServices.DllImport("user32.dll", CharSet=System.Runtime.InteropServices.CharSet.Unicode, SetLastError=true)]
public static extern uint RegisterClipboardFormat(string lpszFormat);
"@

# --- Win32 P/Invoke: kernel32 ---
Add-Type -Namespace Win32 -Name Kernel32 -MemberDefinition @"
[System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError=true)]
public static extern System.IntPtr GlobalLock(System.IntPtr hMem);

[System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError=true)]
public static extern bool GlobalUnlock(System.IntPtr hMem);

[System.Runtime.InteropServices.DllImport("kernel32.dll", SetLastError=true)]
public static extern System.UIntPtr GlobalSize(System.IntPtr hMem);
"@

# --- Helpers ---
function Get-FormatName([uint32]$fmt) {
  $sb = New-Object System.Text.StringBuilder 256
  $r  = [Win32.User32]::GetClipboardFormatName($fmt, $sb, $sb.Capacity)
  if ($r -gt 0) { return $sb.ToString() }
  switch ($fmt) {
    1 {"CF_TEXT"} 2 {"CF_BITMAP"} 3 {"CF_METAFILEPICT"} 4 {"CF_SYLK"}
    5 {"CF_DIF"} 6 {"CF_TIFF"} 7 {"CF_OEMTEXT"} 8 {"CF_DIB"}
    9 {"CF_PALETTE"} 10 {"CF_PENDATA"} 11 {"CF_RIFF"} 12 {"CF_WAVE"}
    13 {"CF_UNICODETEXT"} 14 {"CF_ENHMETAFILE"} 15 {"CF_HDROP"} 16 {"CF_LOCALE"}
    17 {"CF_DIBV5"} default {"#$fmt"}
  }
}

function Read-HGlobalBytes([IntPtr]$hMem) {
  if ($hMem -eq [IntPtr]::Zero) { return $null }
  $ptr = [Win32.Kernel32]::GlobalLock($hMem)
  if ($ptr -eq [IntPtr]::Zero) { return $null }
  try {
    $sz  = [Win32.Kernel32]::GlobalSize($hMem).ToUInt32()
    $buf = New-Object byte[] $sz
    [System.Runtime.InteropServices.Marshal]::Copy($ptr, $buf, 0, $sz) | Out-Null
    return $buf
  } finally {
    [Win32.Kernel32]::GlobalUnlock($hMem) | Out-Null
  }
}

function Try-Read-UnicodeText {
  param([switch]$Show)
  $CF_UNICODETEXT = 13
  $h = [Win32.User32]::GetClipboardData($CF_UNICODETEXT)
  if ($h -eq [IntPtr]::Zero) { return $null }
  $ptr = [Win32.Kernel32]::GlobalLock($h)
  if ($ptr -eq [IntPtr]::Zero) { return $null }
  try {
    $s = [System.Runtime.InteropServices.Marshal]::PtrToStringUni($ptr)
    if ($Show) {
      "`n[CF_UNICODETEXT] (first 500 chars):"
      ($s.Substring(0, [Math]::Min(500, $s.Length)))
    }
    return $s
  } finally {
    [Win32.Kernel32]::GlobalUnlock($h) | Out-Null
  }
}

function Try-Read-HtmlFormat {
  param([switch]$Show)
  $CF_HTML = [Win32.User32]::RegisterClipboardFormat("HTML Format")
  if ($CF_HTML -eq 0) { return $null }
  $h = [Win32.User32]::GetClipboardData($CF_HTML)
  if ($h -eq [IntPtr]::Zero) { return $null }
  $bytes = Read-HGlobalBytes $h
  if ($bytes -eq $null) { return $null }

  if ($Show) {
    "`n[HTML Format] (first 500 bytes as ASCII):"
    $preview = [System.Text.Encoding]::ASCII.GetString($bytes, 0, [Math]::Min(500, $bytes.Length))
    $preview
  }
  return $bytes
}

# --- Main ---
[Win32.User32]::OpenClipboard([IntPtr]::Zero) | Out-Null
try {
  "Formats present:"
  $fmt = 0
  while ($true) {
    $fmt = [Win32.User32]::EnumClipboardFormats($fmt)
    if ($fmt -eq 0) { break }
    "{0}: {1}" -f $fmt, (Get-FormatName $fmt)
  }

  Try-Read-UnicodeText -Show | Out-Null
  Try-Read-HtmlFormat   -Show | Out-Null
}
finally {
  [Win32.User32]::CloseClipboard() | Out-Null
}
