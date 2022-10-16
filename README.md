
# Dupnix 3

It's newer version of [DUPNIX3000](https://github.com/vende11s/dupnix3000), I needed to rebuild it, because i'm still learning programming and decided that building it from scratch would be better option than developing the old shitty code. 

## What is Dupnix 3?
Dupnix 3 is a **backdoor** you can use to gain control on a pc, troll sb, do a lot of things, the only limit is your imagination. If you had any problems with using Dupnix 3 - dm me, my discord is in my bio.
## Safety
Dupnix communicates to you through telegram, so nobody can detect who's on the other side.

## Contents
- [Instalation](#Instalation)
    - [Telegram](#Telegram)
    - [Installation on victim pc](#Instalation-on-victim's-pc)
- [Usage](#Usage)
    - [Syntax](#Syntax)
    - [Commands](#Commands)
- [Todo](#Todo)
- [Contribution](#Contribution)
- [Why Dupnix 3?](#Why-Dupnix-3?)
- [Disclaimer](#Disclaimer)

## Instalation
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
- You need to create a json file, [dupnix-config.json](Dupnix%203/dupnix-config.json) is a template
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

### [Commands](commands.md)
## Todo
- [ ]  make readme look better
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
