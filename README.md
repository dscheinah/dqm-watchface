# DQM Gamified Watchface

This is a fan art inspired by Dragon Quest Monsters (aka Dragon Warrior Monsters).

The watchface displays various watch related information and in addition three monsters.

The monster selection can be done by breeding in the settings page.

In order to breed you need power, gained by sleeping. You will unlock new monsters by walking. These values are updated once an hour and on watchface reload.

All game related information is taken from
https://www.woodus.com/den/gallery/graphics/dwm1gbc/guide/DWMonsters1_Prima.pdf

All assets have been taken from the original game with no credits to myself.

# App-Store

https://apps.rebble.io/en_US/application/6503faaa81ffd2007375874c

# Development

Use the `bootstrap.sh` to prepare the resources from https://www.spriters-resource.com/game_boy_gbc/dwmons/. For aplite support and fitting sprites also `convert` from ImageMagick is needed.

The `docker-compose.yml` provides a setup for the SDK by configuring https://hub.docker.com/r/bboehmke/pebble-dev. Use it with:
`docker-compose run --rm sdk`.

If you are not able to use Docker (e.g. the emulator does not work on Windows), you can download a prebuilt VM disk for https://www.virtualbox.org/ from https://willow.systems/pebble/vm/.
