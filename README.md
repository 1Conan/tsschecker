# tsschecker  
_tsschecker is a powerful tool to check TSS signing status of various devices and iOS/iPadOS/tvOS/watchOS versions._

Latest compiled version can be found here: https://github.com/tihmstar/tsschecker/releases.

Follow [this guide](https://dev.to/jake/using-libcurl3-and-libcurl4-on-ubuntu-1804-bionic-184g) to use tsschecker on Ubuntu 18.04 (Bionic) as it requires libcurl3 which cannot coexist with libcurl4 on this OS.

## Features  
* Allows you to get lists of all devices and all iOS/OTA versions for a specific device.
* Can check signing status for default iOS versions and beta ipsws (by specifying a `BuildManifest.plist`)
* Works without specifying any device relevant values to check signing status, but can be used to save blobs when given an ECID and the option `--print-tss-response` (although there are better tools to do this).

tsschecker is not only meant to be used to check firmware signing status, but also to explore Apple's TSS servers.
By using all of its customization possibilities, you might discover a combination of devices and iOS versions that is now getting signed but wasn't getting signed before. 

### About nonces:
_default generators for saving tickets:_
* `0xbd34a880be0b53f3` // used on Electra & Chimera jailbreaks
* `0x1111111111111111` // used on unc0ver jailbreak

Nonce collision method isn't needed anymore, because we've [checkm8](https://github.com/axi0mx/ipwndfu) low-level exploit.

# Dependencies
*  ## Bundled libs
  Those don't need to be installed manually
  * [tss](https://github.com/libimobiledevice)
* ## External libs
  Make sure these are installed
  * [libcurl](https://curl.haxx.se/libcurl/)
  * [libplist](https://github.com/libimobiledevice/libplist)
  * [libfragmentzip](https://github.com/tihmstar/libfragmentzip)
  * [openssl](https://github.com/openssl/openssl) or commonCrypto on macOS/OS X;
  * [libirecovery](https://github.com/libimobiledevice/libirecovery);
* ## Submodules
  Make sure these projects compile on your system
  * [jssy](https://github.com/tihmstar/jssy)

## Help  
_(might become outdated):_

Usage: `tsschecker [OPTIONS]`

| option (short) | option (long)             | description                                                                       |
|----------------|---------------------------|-----------------------------------------------------------------------------------|
|  `-h`          | `--help`                  | prints usage information                                                          |        
|  `-d`          | `--device MODEL`          | specify device by its model (eg. `iPhone4,1`)                                     |
|  `-i`          | `--ios VERSION`           | specify firmware version (eg. `6.1.3`)                                                 |
|  `-Z`	   | `--buildid BUILD `	| specific buildid instead of firmware version (eg. `13C75`)							               |
|  `-B` 	   | `--boardconfig BOARD `	   | specific boardconfig instead of device model (eg. `n61ap`)						             |
|  `-o`          | `--ota`	                 | check OTA signing status, instead of normal restore                               |
|  `-b`          | `--no-baseband`           | don't check baseband signing status. Request a ticket without baseband            |
|  `-m`          | `--build-manifest`   | manually specify buildmanifest (can be used with `-d`)                           | 
|  `-s`          | `--save`		     		       | save fetched shsh blobs (mostly makes sense with -e)                              |
|  `-u`          | `--update-install         `| request update ticket instead of erase                          |  
|  `-l`	   | `--latest`  				       | use latest public firmware version instead of manually specifying one<br>especially useful with `-s` and `-e` for saving signing tickets                                                                                              |
|  `-e`          | `--ecid ECID`	         | manually specify an ECID to be used for fetching blobs, instead of using random ones. <br>ECID must be either DEC or HEX eg. `5482657301265` or `ab46efcbf71`                                                          |
|  `-g`          | `--generator GEN`        | manually specify generator in format 0x%%16llx                                                                                                        |
|      			     | `--apnonce NONCE`   		   | manually specify ApNonce instead of using random one (not required for saving signing tickets) |
|      			     | `--sepnonce NONCE`        | manually specify SepNonce instead of using random one (not required for saving signing tickets) 		                                                                                                                                  |
|                           | `--bbsnum SNUM`        | manually specify BbSNUM in HEX for saving valid BBTicket (not required for saving blobs)                                                                                                                                   |
|      			     | `--save-path PATH`        | specify path for saving blobs 		 											 |
|                |`--beta`	             | request ticket for beta instead of normal release (use with `-o`)                |
|                |`--list-devices`          | list all known devices                                                            |
|                |`--list-ios`	             | list all known firmware versions                                                       |
|                |`--nocache`       	     | ignore caches and re-download required files                                      |
|                |`--print-tss-request`      | prints TSS request that will be sent to Apple                                      |
|                |`--print-tss-response`     | prints TSS response that come from Apple                                  |
|                |`--raw`     | send raw file to Apple's TSS server (useful for debugging)                                 |
