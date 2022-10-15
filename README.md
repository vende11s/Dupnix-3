
# Dupnix 3

It's newer version of [DUPNIX3000]("https://github.com/vende11s/dupnix3000"), I needed to rebuild it, because i'm still learning programming and decided that building it from scratch would be better option than developing the old shitty code. 

## What is Dupnix 3?
Dupnix 3 is a **backdoor** you can use to gain control on a pc, troll sb, do a lot of things, the only limit is your imagination. If you had any problems with using Dupnix 3 - dm me, my discord is in my bio.
## Safety
Dupnix communicates to you through telegram, so nobody can detect who's on the other side.
## Installation
### Telegram
#### Create a bot
- Start with texting `/newbot` to `@BotFather` on Telegram
- Name your bot 
- `BotFather` will send you ur **api token**, you'll need it later
- Text to him `/setPrivacy`, select ur bot and text him `Disable`
- Create a channel and add ur bot as admin
#### Get `chat_id`
- Text anything on the channel u created
- go to `https://api.telegram.org/bot<api Token>/getUpdates` 
- Find the `"id":` number, that's ur `chat_id`
### Json
- You need to create a json file, [dupnix-config.json]("Dupnix%203/dupnix-config.json") is a template
    - `autostart <true/false>` automatically installs the Dupnix and runs it every pc boot
    - `hide_terminal <true/false>` who would expect that it hides the terminal?
    - `refresh_rate <seconds>` how often Dupnix checks if you sent some message
    - `id` sets id of this pc (useful when u controll more than one pc) 
    - `bot_token` token of bot you can get in [Telegram]("###Telegram") section
    - `chat_id` id of ur chat, get it in the [Telegram]("###Telegram") section
    - `path` path where u want the Dupnix to be (you can use `%sysdisk%` and `%username%`)

### Instalation on victim's pc
- Just run executable (in same directory as json file) and enjoy causing chaos!
## Usage

#### Syntax u send on the telegram channel
 * `<ID> <Command> <Parameters>`

### Commands
#### ALL_ID
* Sends you id of all pc's you've got control on (the only command you don't use the `ID`)
#### Function List
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
#### SetID <ID>
* Changes ID
#### Cmd <command>
* just execute a cmd command
#### CmdOutput <command>
* execute and get output of a cmd command
#### Volume <>/<number>/<+number>/<-number>
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
#### ChangeCfg <json-syntax change>
* Changes value in config file, example
```
<ID> ChangeCfg {"id": "2137"}
```
#### SetCursor <x,y>
* Sets cursor to given position
#### Delay <seconds command parameters>
* Performs a command with delay (you cannot perform any other command until this one is done)
#### BlockCursor <seconds>
* Sets cursor to `0,0` for `<seconds>`
#### BlockClipboard <true/false>
* Blocks copying and pasting
#### SendClipboard
* if there is some text in clipboard, it sends it to u
#### WriteToClipboard <text>
* Puts some text to clipboard
#### Press <text>
* Writes on clipboard some text
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
#### IsFileExists <path>
* says if given file exists 
#### WebcamView
* Sends you a screenshot from webcam
#### ErrorSound
* makes a windows error sound 
#### RunningApps
* List of first-plan RunningApps 
#### ListOfFiles <path>
* works like a linux `ls` (here path is required)
#### WifiList
* sends you list of saved wifi and passwords to them
#### CloseForeground <true/false>
* A troll that closes every window u click at
## Todo
- [ ]  first run of dupnix with args to chat_id and botid
- [ ]  when there is no ID give it random one
- [ ]  prepare rubber ducky script
- [ ]  change dupnix-config.json to something less suspicious
- [ ]  when there is bad syntax in ChangeCfg, dupnix crashes
- [ ]  after ChangeCfg, load cfg again
- [ ]  web browser saved passwords
## Contribution
If you wanna do something then - contribute, before text me (my discord is in my bio) and we can work together!
## Why Dupnix 3?
* Why Dupnix 3 is called Dupnix 3? On one of cash machines in my town there is sign DUPNIX, that's all.

## Disclaimer
Whole this project is only for eductional purposes, I am not responsible for anything you did or will do with this software.
