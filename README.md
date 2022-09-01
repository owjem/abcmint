Abcmint integration/staging tree
=====================================

http://www.abcmint.org

Copyright (c) 2018 ABCMint Core Developers

What is Abcmint?
----------------

Abcmint is an experimental new digital currency that enables instant payments to
anyone, anywhere in the world. Abcmint uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Abcmint is also the name of the open source
software which enables the use of this currency.

For more information, as well as an immediately useable, binary version of
the ABCMintCore client software, see http://www.abcmint.org.

License
-------

ABCMintCore is released under the terms of the GNU GPL v. 3 license. See https://www.gnu.org/licenses/gpl-3.0.en.html for more information.

THIRD PARTY SOFTWARE DISCLOSURE, ATTRIBUTIONS, COPYRIGHT NOTICES, AND LICENSES

Certain open source third-party software and data components are integrated and/or redistributed with
ABCMintCore. Such third-party components include terms and conditions, such as attribution and liability disclaimers (collectively "Third Party Disclosures") for which disclosure is required by their respective owners. This notice sets forth such Third Party Disclosures for ABCMintCore.

ABC MINT FOUNDATION MAKES NO REPRESENTATION, WARRANTY OR OTHER COMMITMENT REGARDING SUCH THIRD PARTY COMPONENTS.  ABCMINTCORE, AND ALL THIRD PARTY COMPONENTS, ARE DISTRIBUTED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, UNLESS PROHIBITED BY APPLICABLE LAW


1. Openssl
•	Copyright (c) 1998-2018 The OpenSSL Project, all rights reserved
•	Copyright (c) 1995-1998 Eric A. Young, Tim J. Hudson, all rights reserved
•	All rights reserved.
•	URL: https://www.openssl.org/
•	Version: 1.1.0g
•	License 1: Original SSLeay License
•	License 2: Apache License 1.0
•	License 3: Apache License 2.0

2. Berkeley DB

•	Copyright (c) 1990, 1993, 1994, 1995	The Regents of the University of California, all rights reserved
•	Copyright (c) 1995, 1996 The President and Fellows of Harvard University, all rights reserved
•	URL: http://www.oracle.com/technetwork/database/database-technologies/berkeleydb/overview/index.html
•	Version: 4.8.30
•	License:  Affero General Public License v.3

3. Level db
•	Copyright (c) 2011 The LevelDB Authors. All rights reserved.
•	URL: http://leveldb.org/
•	Version:1.1
•	License:  BSD License Clause 3

4. Boost
•	Copyright Beman Dawes, David Abrahams, 1998-2005.
•	Copyright Rene Rivera 2004-2005.
•	URL: https://www.boost.org/
•	Version: 1.65.0
•	License:  Boost Software License

5. libpng
•	libpng versions 1.0.7, July 1, 2000 through 1.6.32, August 24, 2017 are Copyright (c) 2000-2002, 2004, 2006-2017 Glenn Randers-Pehrson, are derived from libpng-1.0.6, and are distributed according to the same disclaimer and license as libpng-1.0.6 with the following individuals added to the list of Contributing Authors:

o	   Simon-Pierre Cadieux
o	   Eric S. Raymond
o	   Mans Rullgard
o	   Cosmin Truta
o	   Gilles Vollant
o	   James Yu
o	   Mandar Sahastrabuddhe
o	   Google Inc.
o	   Vadim Barkov
•	URL: http://libpng.org/
•	Version: 1.6.16
•	License: libpng license

6. Protocol buffer
•	Copyright 2008 Google, Inc.
•	URL: https://developers.google.com/protocol-buffers/
•	Version: 2.6.1
•	License: BSD

7. qrencode
•	Copyright 2006-2017 Kentaro Fukuchi kentaro@fukuchi.org.
•	URL: https://fukuchi.org/works/qrencode/manual/index.html
•	Version: 3.4.4
•	License:  GNU Lesser General Public License 2.1

8. Qt
•	Copyright Various Years, QT Company
•	URL: www.qt.io
•	Version: 5.6.3
•	License GPL v. 2; GPL v. 3; : LGPL v. 3

9. MiniUPnP
•	Copyright (c) 2006-2017, Thomas BERNARD, all rights reserved
•	URL:http://miniupnp.free.fr/
•	Version: 2.1
•	License: MiniUPnP License

10. Bitcoin Core
•	Copyright (c) 2009-2018 The Bitcoin Core developers
•	URL:https://bitcoin.org/en/bitcoin-core/
•	Version:0.8.6
•	License: MIT License

11. libFES algorithm
•	URL: http://www.lifl.fr/~bouillag/fes/index.html
•	Version:0.2
•	GPL V3



Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the ABCMintCore
development team members simply pulls it.

If it is a *more complicated or potentially controversial* change, then the patch
submitter will be asked to start a discussion (if they haven't already) on the
[mailing list](http://sourceforge.net/mailarchive/forum.php?forum_name=abcmint-development).

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see `doc/coding.md`) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/abcmint/abcmint/tags) are created
regularly to indicate new official, stable release versions of Abcmint.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### build

```
./autogen.sh
CONFIG_SITE=./depends/x86_64-pc-linux-gnu/share/config.site ./configure --without-qt
make
```

Dependency Build Instructions: Ubuntu & Debian
-------
```
cd depends
make clean
make
```

about QT GUI
-------
There is no time to work on QT GUI, so we can deal with it when necessary. Now we mainly use our spare time to merge part of the code of the new version of bitcoin into abcmint

About Mining Using GPU
-------
-genproclimit=$GPUNUM, The parameter is the number of GPUs

```
./abcmintd -rpcuser=abcmintrpc -rpcpassword=yourpassword -gen=1 -genproclimit=$GPUNUM
```
or
```
./abcmintd -rpcuser=abcmintrpc -rpcpassword=yourpassword -daemon
./abcmint-cli -rpcuser=abcmintrpc -rpcpassword=yourpassword setgenerate true,$GPUNUM
```
