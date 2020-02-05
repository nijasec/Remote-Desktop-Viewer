Dim WShell
Set WShell = CreateObject("WScript.Shell")
WShell.Run "remote_desktop_monitor_test.exe", 0
Set WShell = Nothing