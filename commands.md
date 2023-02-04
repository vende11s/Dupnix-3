### Commands
* *You dont need to care about upper/lowercase in commands names
#### ALL_ID
* Sends you id of all pc's you've got control on (the only command you don't use the `ID`)
#### FunctionList
* Quick reminder of all functions (sends all names of functions)
#### Status
* Sends some useful stuff about pc
example:
```
uptime: 0h0m
exe_name: Dupnix 3.exe
host_name: <host_name> 
public_ip: <ip>
local_ip: 
  Ethernet: 169.254.105.192
  Wi-Fi: 192.168.1.103
admin_rights: 0
exe_path: C:\Users\<username>\source\repos\Dupnix 3\Debug\Dupnix 3.exe
current_volume: 6
cursor_position: 960,400
list_of_disks: C, D, E, F

version: v0.1
```

### `<ID>`
* Returns Status
#### SetID `<ID>`
* Changes ID
#### Cmd `<command>`
* just execute a cmd command
#### CmdOutput `<command>`
* execute and get output of a cmd command
#### Volume `<>/<number>/<+number>/<-number>`
* Parameters `empty` - just sends current Volume
* `Number` in parameters - sets volume to this number
* `+Number` in parameter - increases volume
* `-Number` in parameter - decreases volume
#### Screenshot
* makes and sends you a Screenshot
#### MonitorOff
* turns monitor off 
#### MonitorOn
* tunrs monitor on 
#### CheckCfg
* Sends you the config file
#### ChangeCfg `<json-syntax change>`
* Changes value in config file, example
```
<ID> ChangeCfg {"id": "2137"}
```
#### SetCursor `<x,y>`
* Sets cursor to given position
#### Delay `<seconds command parameters>`
* Performs a command with delay (you cannot perform any other command until this one is done)
#### BlockCursor `<seconds>`
* Sets cursor to `0,0` for `<seconds>`
#### BlockClipboard `<true/false>`
* Blocks copying and pasting
#### SendClipboard
* if there is some text in clipboard, it sends it to u
#### WriteToClipboard `<text>`
* Puts some text to clipboard
#### Press `<text>`
* Works like BadUsb's STRING, just writes some text like it was user clicking keyboard
example:
`<ID> Press JDJD`
#### Hotkey <hotkey>
example:
` <ID> Hotkey ctrl+z`
* Special keys u can use:
```
tab
capslock
shift
ctrl
win
alt
ralt
space
enter
backspace
del
f1
f..12
ins
home
end
pgdn
pgup
uparrow
downarrow
leftarrow
rightarrow
lmouse 
```
#### Autodestruction
* Deletes Dupnix and all files
#### ProcessList
* Sends you list of all processes
#### IsFileExists `<path>`
* says if given file exists 
#### WebcamView
* Sends you a screenshot from webcam
#### ErrorSound
* makes a windows error sound 
#### RunningApps
* List of first-plan RunningApps 
#### ListOfFiles `<path>`
* works like a linux `ls` (here path is required)
#### WifiList
* sends you list of saved wifi and passwords to them
#### CloseForeground `<true/false>`
* A troll that closes every window u click at
#### DownloadFile `<link>`
* Downloads a file to dupnix directory
#### SendFile` <path>`
* Sends u a file from given path
#### shell
* Nothing more than just very simple shell
#### UpdateDupnix `<link>`
* Replaces Dupnix executable with given executable from link eg. `https://github.com/vende11s/Dupnix-3/releases/download/v1.0/Dupnix-3.exe`
