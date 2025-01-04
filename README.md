# tsschecker
[![CI Building](https://img.shields.io/github/actions/workflow/status/1Conan/tsschecker/build_release.yml?branch=main&style=for-the-badge)](https://github.com/1Conan/tsschecker/actions)

tsschecker is a powerful tool to check TSS signing status on combinations of various apple devices and firmware versions.

## Features  
* Supports: Apple TV, Apple Watch, HomePod, iPad, iPhone, iPod touch, M1 Macs and the T2 Coprocessor.
* Allows you to get lists of supported apple devices as well as Firmwares and OTA versions for any specified apple device.
* Can check signing status for any firmware version by specifying either a firmware version or a BuildManifest.
* Works without specifying any device relevant values to check signing status, but can be used to save blobs when given an ECID and the option --print-tss-response (although there are better tools to do this).

tsschecker is not only meant to be used to check firmware signing status, but also to explore Apple's TSS servers.<br/>
By using all of its customization possibilities, you might discover a combination of devices and firmware versions that is getting signed but wasn't getting signed before. 

# About Nonces:
A [Nonce](https://wikipedia.org/wiki/Cryptographic_nonce) ("Number-used-ONCE") is a randomly generated value that is used to randomize apple's signed hash blobs.

it is created by the device with a nonce seed (generator) and then hashes that seed to create the nonce.<br/>On arm64e devices, nonce generation works a bit differently, see "Nonce Entangling" for more details.

## Recommended nonce-seeds (Generators) for saving tickets:
* `0xbd34a880be0b53f3` // default on the [Electra](https://coolstar.org/electra/), [Chimera](https://chimera.coolstar.org/) and [Odyssey](https://theodyssey.dev/) jailbreak apps.
* `0x1111111111111111` // default on the [unc0ver](https://unc0ver.dev) jailbreak app.

## Nonce Entangling (arm64e devices)
arm64e devices such as the iPhone XR, Apple Watch Series 4 and all newer devices have nonce-entangling.

Nonce Entangling further randomizes the boot nonce by encrypting a constant value with the [UID key](https://www.theiphonewiki.com/wiki/UID_key) (using AES-256-CBC)<br/>It is then used to encrypt the "generator" value (using AES-128-CBC) before hashing it to become the boot nonce.

In short, nonce entangling makes the boot nonce unique to that device only.

To save reusable tickets for any arm64e device, you must get the boot nonce that the device creates from your generator,<br/>
the simpliest way to get a nonce/generator pair is to use airsquared's [blobsaver](https://github.com/airsquared/blobsaver) tool and read them from the device.<br/>
If you are jailbroken, another tool than can do this is [x8A4](https://github.com/Cryptiiiic/x8A4). You dump the 0x8A3 key to encrypt the nonce generator.<br/>
To encrypt the nonce generator, first dump the 0x8A3 key with `sudo x8A4 -k 0x8A3`. You can then use the [AES Nonce](https://github.com/Cryptiiiic/aes_nonce) tool to encrypt the generator.<br/>
`python3 aes_nonce.py <Dumped 0x8A3 Key> <APNonce Generator>`<br/>
`python3 aes_nonce.py C5DA157E11DE12664CD9C702C21B8ECC 1111111111111111` -> `04faa44b7c7826c840aebd9c9aaacbff948c90eb743e6118f6bb875178ca8b14`<br/>
Then finally you can save valid arm64e blobs like so:<br/>
`tsschecker --device iPhone11,8 --boardconfig n841ap --ecid 0x69 -g 0x1111111111111111 -8 04faa44b7c7826c840aebd9c9aaacbff948c90eb743e6118f6bb875178ca8b14 -l -E -s`<br/>


For more information, visit The iPhone Wiki:<br/>
[The iPhone Wiki](https://www.theiphonewiki.com/) ([AES Keys](https://theiphonewiki.com/wiki/AES_Keys), [Nonce](https://theiphonewiki.com/wiki/Nonce))

## Cryptex1 Blobs
iOS 16 added a new component with its own seed(generator) and nonce called Cryptex1. The seed gets entangled even on a10(X)/a11 devices using the 0x8A4 key.<br/>
A jailbreak is **REQUIRED** to save and use cryptex blob. In fact, checkm8 need needed to even use cryptex blobs!<br/>
If you are jailbroken, you can use the [x8A4](https://github.com/Cryptiiiic/x8A4) tool to encrypt the cryptex seed. You dump the 0x8A4 key to encrypt the cryptex seed.<br/>
To encrypt the seed, first dump the 0x8A4 key with `sudo x8A4 -k 0x8A4`. You can then use the [AES Nonce](https://github.com/Cryptiiiic/aes_nonce) tool to encrypt the seed.<br/>
Usage:<br/>
`python3 aes_cryptex_nonce.py <Dumped 0x8A4 Key> <Cryptex Seed>`<br/>
`python3 aes_cryptex_nonce.py aes_cryptex_nonce.py DF6A9324032C86159F0DE3A1D477B3F2 11111111111111111111111111111111` -> `f7cfa05f0207570426e6c96af9a8da73eeb15a17341a1d09244a3ea05b7b5077`<br/>
Then finally you can save cryptex blobs like so:<br/>
`tsschecker --device iPhone10,3 --boardconfig d22ap --ecid 0x69 -g 0x1111111111111111 -x 0x11111111111111111111111111111111 -t f7cfa05f0207570426e6c96af9a8da73eeb15a17341a1d09244a3ea05b7b5077 -l -E -s`<br/>


## Nonce Collisions:

the Nonce Collision method only works on a few firmwares and devices, and is not reliable and not recommended.<br/>it's a lot better to save a ticket with a generator and use the [checkm8](https://github.com/axi0mx/ipwndfu) bootrom exploit or a nonce setter.

Recovery Nonce Collisions only occur on a few iOS versions, like iOS 9.3.3 and iOS 10.1-10.2 on the iPhone 5s<br/>and is not reliable as once you update, your device will almost-certainly not collide nonces anymore.

DFU Nonce Collisions on the other hand, very commonly occur on any device using A7 and A8 chipsets regardless of iOS version,<br/> and is MUCH more reliable than using recovery collisions.

# Build
Install or Compile dependencies

* Buildsystem:
  * autoconf
  * autoconf-archive
  * autogen
  * automake
  * libtool
  * m4
  * make
  * pkg-config

* Tihmstar's libs:
  * [libgeneral](https://github.com/tihmstar/libgeneral)
  * [libfragmentzip](https://github.com/tihmstar/libfragmentzip)

* External libs:
  * [libcurl](https://curl.haxx.se/libcurl/)
  * [libirecovery](https://github.com/libimobiledevice/libirecovery)
  * [libplist](https://github.com/libimobiledevice/libplist)
  * [libzip](https://libzip.org/)
  * [openssl](https://www.openssl.org/) (or you can use CommonCrypto on macOS/OS X)
  * [zlib](https://zlib.net/)
  
* Submodules:
  * [jssy](https://github.com/tihmstar/jssy)
  
* Bundled libs, (not required to be installed manually):
  * [tss](https://github.com/libimobiledevice)

To compile, run:

```bash
./autogen.sh
make
sudo make install
```

# Help  
Usage: `tsschecker [OPTIONS]`

Example: `tsschecker -d iPhone10,1 -B D20AP -e 5482657301265 -i 14.7.1 --generator 0x1111111111111111 -s`

| option<br/>(short) | option<br/>(long) | description |
|-------|-------------------------|--------------------|
| `-h`  | `--help`                | prints usage information |
| `-d`  | `--device MODEL`        | specify device by its model (eg. `iPhone10,3`) |
| `-i`  | `--ios VERSION`         | specify firmware version (eg. `14.7.1`) |
| `-Z`  | `--buildid BUILD`       | specific buildid instead of firmware version (eg. `18G82`) |
| `-B`  | `--boardconfig BOARD`   | specific boardconfig instead of device model (eg. `d22ap`) |
| `-o`  | `--ota`                 | check OTA signing status, instead of normal restore |
| `-b`  | `--no-baseband`         | don't check baseband signing status. Request tickets without baseband |
| `-m`  | `--build-manifest`      | manually specify a BuildManifest (can be used with `-d`) |
| `-s`  | `--save`                | save fetched shsh blobs (mostly makes sense with -e) |
| `-u`  | `--update-install`      | only request tickets for InstallType=Update |
| `-E`  | `--erase-install`       | only request tickets for InstallType=Erase |
| `-l`  | `--latest`              | use the latest public firmware version instead of manually specifying one<br/>especially useful with `-s` and `-e` for saving signing tickets |
| `-e`  | `--ecid ECID`           | manually specify ECID to be used for fetching blobs, instead of using random ones.<br/>ECID must be either DEC or HEX eg. `5482657301265` or `ab46efcbf71` |
| `-g`  | `--generator GEN`       | manually specify generator in format 0x%%16llx |
| `-8`  | `--apnonce NONCE`       | manually specify ApNonce instead of using random ones<br/>(required when saving blobs for arm64e devices with a matching generator) |
| `-9`  | `--sepnonce NONCE`      | manually specify SepNonce instead of using random ones (not required for saving signing tickets) |
| `-c`  | `--bbsnum SNUM`         | manually specify BbSNUM in HEX to save valid BBTickets (not required for saving blobs) |
| `-3`  | `--save-path PATH`      | specify path for saving shsh blobs |
| `-6`  | `--beta`                | request ticket for a beta instead of normal release (use with `-o`) |
| `-1`  | `--list-devices`        | list all known devices |
| `-2`  | `--list-ios`            | list all known firmware versions |
| `-7`  | `--nocache`             | ignore caches and re-download required files |
| `-4`  | `--print-tss-request`   | print the TSS request that will be sent to Apple |
| `-5`  | `--print-tss-response`  | print the TSS response that comes from Apple |
| `-r`  | `--raw`                 | send raw file to Apple's TSS server (useful for debugging) |
| `-0`  | `--debug`               | print extra tss info(useful for debugging) |
