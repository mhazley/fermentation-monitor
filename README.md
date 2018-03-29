# fermentation-monitor

## A Particle Photon based fermentation chamber temperature controller for home brewers

### Setup

Make sure that you update the submodules using `git submodule update`

### Building

To build the firmware
`make`

To flash the system firmware with the pinned version for this repo, build the firmware, & flash the Photon
`make upgrade`
> [Enter DFU mode](https://docs.particle.io/guide/getting-started/modes/photon/#dfu-mode-device-firmware-upgrade-) on the Photon first
