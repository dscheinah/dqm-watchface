# DQM Gamified Watchface

This is a fan art inspired by Dragon Quest Monsters (aka Dragon Warrior Monsters).

Gamification is still subject to be added in a later version.

All assets have been taken from the original game with no credits to myself.

# App-Store

TODO

# Development

Use the `bootstrap.sh` to prepare the resources from https://www.spriters-resource.com/game_boy_gbc/dwmons/. For aplite support and fitting sprites also `convert` from ImageMagick is needed.

The `docker-compose.yml` provides a setup for the SDK by configuring https://hub.docker.com/r/bboehmke/pebble-dev. Use it with:
`docker-compose run --rm sdk`.

If you are not able to use Docker (e.g. the emulator does not work on Windows), you can download a prebuilt VM disk for https://www.virtualbox.org/ from https://willow.systems/pebble/vm/.
