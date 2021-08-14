# tsschecker

`tsschecker` is a powerful tool to check TSS signing status on combinations of Apple devices and firmware versions.

## Features

`tsschecker` is not only meant to be used to check firmware signing status, but also to explore Apple's TSS server. Not only that, but `tsschecker` can

- Check signing status by specifying a firmware version or a BuildManifest, without any device relevant values
- Get a list of supported Apple devices for a specific firmware or OTA version, for any specific Apple device
- Save blobs for a specific Apple device given the device's ECID (though there are better tools to do this)

`tsschecker` supports most Apple devices, including iPads, iPhones, M1 Macs, and the T2 Coprocessor. By using all of its customization possibilities, you might discover a combination of devices and firmware versions that are getting signed at an unexpected time.

## Building

The following dropdowns list the neeeded dependencies for `tsschecker` to build. You'll need them installed on your target system.

<details>
  <summary><strong>Build system</strong></summary><br>
  
  - autoconf
  - autoconf-archive
  - autogen
  - automake
  - libtool
  - m4
  - make
  - pkg-config
  ##
</details>
<details>
  <summary><strong>timstar's Libraries</strong></summary><br>
  
  - [libgeneral](https://github.com/tihmstar/libgeneral)
  - [libfragmentzip](https://github.com/tihmstar/libfragmentzip)
  - [jssy](https://github.com/tihmstar/jssy)
  ##
</details>
<details>
  <summary><strong>Extenal Libraries</strong></summary><br>
  
  - [libcurl](https://curl.haxx.se/libcurl/)
  - [libirecovery](https://github.com/libimobiledevice/libirecovery)
  - [libplist](https://github.com/libimobiledevice/libplist)
  - [libzip](https://libzip.org/)
  - [OpenSSL](https://www.openssl.org/) (or use CommonCrypto on macOS/OSX)
  - [zlib](https://zlib.net/)
  ##
</details>

To build `tsschecker`, run the following

    ./autogen.sh
    make
    sudo make install

## Usage

`tsschecker` can be used by providing arguments, or options after its reference, as shown below

    tsschecker [OPTIONS]

This section details the usage of `tsschecker`, and the many options its CLI offers, below.

| Option (short) | Option (long) | Description |
|----------------|---------------|-------------|
| `-h` | `--help` | Prints usage information |
| `-d` | `--device MODEL` | Specifies device model (e.g, `iPhone8,1`) |
| `-i` | `--ios VERSION` | Specifies firmware version (e.g, `13.4.1`) |
| `-Z` | `--buildid BUILD` | Specifies BuildID instead of firmware version (e.g, `17E255`) |
| `-B` | `--boardconfig BOARD` | Specifies boardconfig instead of device model (e.g, `n71ap`) |
| `-o` | `--ota` | Checks OTA signing status instead of normal restore |
| `-b` | `--no-baseband` | Request tickets without baseband; don't check baseband signing status |
| `-m` | `--build-manifest` | Manually specify a BuildManifest (can be used with `-d`) |
| `-s` | `--save` | Saves fetched SHSH tickets, or "blobs" (mostly used with `-e`) |
| `-u` | `--update-install` | Requests update tickets instead of erase |
| `-l` | `--latest` | Use the latest public firmware version instead of manually specifying a firmware version. Especially useful with `-s` and `-e` for saving tickets, or "blobs" |
| `-e` | `--ecid ECID` | Manually specifies an ECID to be used for fetching blobs instead of using a random one. Acceptable ECID values can be either DEC or HEX (e.g, `5482657301265` or `ab46efcbf71`) |
| `-g` | `--generator GENERATOR` | Manually specifies a device generator (DEC or HEX) |
|      | `--apnonce NONCE` | Manually specifies an APNonce instead of using a random one (required to save tickets for chipsets A12, S4, and newer) |
|      | `--sepnonce NONCE` | Manually specifies an SEPNonce instead of using a random one (not required for saving blobs) |
|      | `--bbsnum SNUM` | Manually specifies BBSNUM in HEX to save valid BBTickets (not required for saving blobs) |
|      | `--save-path PATH` | Specfies path where SHSH tickets, or "blobs" should be dumped once requested |
|      | `--beta` | Requests tickets for beta releases instead of normal releases (similar to `--ota`) |
|      | `--list-devices` | List all known devices |
|      | `--list-ios` | List all known firmware version |
|      | `--nocache` | Ignore caches and re-download required files |
|      | `--print-tss-request` | Print the TSS request that is sent to Apple's servers |
|      | `--print-tss-response` | Print the TSS response that comes from Apple's servers |
|      | `--raw` | Send a raw file to Apple's TSS server (mostly used for debugging) |

## About Nonces

This section entails most information about "nonces", nonce generators, and everything in between.

### Recommended Generators

The following nonce generators are recommended to be used when saving tickets, or "blobs".

- `0x1111111111111111`: default on the unc0ver jailbreak app
- `0xbd34a880be0b53f3`: default on the Electra, Chimera, and Odyssey jailbreak apps

While these generators are specific to several jailbreaks, they're not limited to these. You can (in theory) use one generator on another jailbreak when saving blobs; these jailbreaks might just save blobs with those specific generators.

The best thing to do is to always remember your nonce generator!

### Nonce Entangling (Apple A12/S4 and newer)

Newer devices, such as the iPhone XR or the Apple Watch series 4 have nonce entangling.

This means any boot nonce generated by these devices is also UID derived, and consequently device specific. To save usable tickets, or "blobs", for these newer devices, you need to get the boot nonce that your device actually generates from your generator.

For more information on how to get the boot nonce for these devices, [see this post or r/jailbreak](https://www.reddit.com/r/jailbreak/comments/cssh8f/tutorial_easiest_way_to_save_blobs_on_a12/).

### Nonce Collisions

Nonce collisions only work on a few firmwares and devices. Saving tickets, or "blobs" this way is not only unreliable, but also not recommended.

Recovery nonce collisions only occur on a few iOS versions, like iOS 9.3.3 and iOS 10.1-10.2 on the iPhone 5s. This information means that the device mentioned above can upgrade to these versions, but it's most certainly not recommended, as devices will almost-certainly not collide nonces anymore once upgrading.

DFU nonce collisions, on the other hand, very commonly occur on any device using A7-A8 chipsets regardless of iOS version and is **much** more reliable than recovery nonce collisions.

## License

This project has been licensed under the [GNU Lesser General Public License v3.0](LICENSE).
