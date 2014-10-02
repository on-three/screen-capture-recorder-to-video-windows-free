#define AppVer "0.12.8pre"

#define AppName "capture-text-overlay"
; AppId === AppName by default BTW

; To use this, run it from within virtual ...

[Run]
Filename: {app}\vendor\vcredist_x86.exe; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing MSVC 2010 RunTime...
Filename: regsvr32; WorkingDir: {app}; Parameters: /s capture-text-overlay.dll

; TODO clear registry, prefs on uninstall?

[UninstallRun]
Filename: regsvr32; WorkingDir: {app}; Parameters: /s /u capture-text-overlay.dll

[Files]
Source: source_code\Win32\Release\capture-text-overlay.dll; DestDir: {app}
Source: README.TXT; DestDir: {app}; Flags: isreadme
Source: ChangeLog.txt; DestDir: {app}
; includes vendor/ffmpeg et al
Source: configuration_setup_utility\*.*; DestDir: {app}\configuration_setup_utility; Flags: recursesubdirs
Source: vendor\troubleshooting_benchmarker\BltTest\Release\BltTest.exe; DestDir: {app}
Source: vendor\vcredist_*.exe; DestDir: {app}\vendor
; ruby scripts read version from this
Source: innosetup_installer_options.iss; DestDir: {app}\


[Setup]
AppName={#AppName}
AppVerName={#AppVer}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
UninstallDisplayName={#AppName} uninstall
OutputBaseFilename=Setup {#AppName} v{#AppVer}
OutputDir=releases

; remove previous versions' outdated icons [lame innosetup, lame]
[InstallDelete]
Type: filesandordirs; Name: {group}\*;

[Icons]
Name: {group}\configure\Release Notes; Filename: {app}\ChangeLog.txt
Name: {group}\configure\Readme; Filename: {app}\README.TXT
Name: {group}\configure\configure by setting specific screen capture numbers; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: setup_via_numbers.rb; Flags: runminimized
Name: {group}\configure\benchmark your machines screen capture speed; Filename: {app}\BltTest.exe; WorkingDir: {app}
Name: {group}\configure\configure by resizing a transparent window; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: window_resize.rb; Flags: runminimized
Name: {group}\configure\Display current capture settings; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: setup_via_numbers.rb --just-display-current-settings
Name: {group}\configure\Uninstall {#AppName}; Filename: {uninstallexe}
Name: {group}\Record\Record or stream video and or audio; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: record_with_buttons.rb; Flags: runminimized
Name: {group}\Record\Record audio by clicking a button; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: record_with_buttons.rb --just-audio-default; Flags: runminimized
Name: {group}\record\broadcast\setup local audio broadcast streaming server; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: broadcast_server_setup.rb; Flags: runminimized
Name: {group}\record\broadcast\restart local audio streaming server with same setup as was run previous; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: broadcast_server_setup.rb --redo-with-last-run; Flags: runminimized
Name: {group}\record\broadcast\stream desktop local LAN; Filename: {app}\configuration_setup_utility\generic_run_rb.bat; WorkingDir: {app}\configuration_setup_utility; Parameters: stream_desktop_p2p.rb; Flags: runminimized;

[Languages]
Name: fr; MessagesFile: compiler:Languages\French.isl; 
Name: de; MessagesFile: compiler:Languages\German.isl; 
Name: "en"; MessagesFile: "compiler:Default.isl"
