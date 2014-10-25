README
======

Downloads
---------
* [VDADecoderChecker Binary](https://github.com/downloads/cylonbrain/VDADecoderCheck/VDADecoderChecker) - Download precompiled binary (Intel 64-Bit):

Summary
----------

Small command line tool that checks the capabilities of your graphics hardware & OS to decode h264 videos on your GPU. Apple doesn't seems to allow all GPU's to use the System Framework, despite the fact that they have the hardware capabilities (like Geforce 8800 or Fermi cards). The tool have the same error codes like other VDA Software (XBMC) but its easier to test.



Apple officaly support Hardware Video Acceleration only on Mac models equipped with: 
GeForce 9400M
GeForce 320M
GeForce GT 330M
ATI HD Radeon GFX
Intel HD Graphics
and newer

Feel free to contribute on Github

License
-------
VDADecoderChecker is released under the GNU General Public License V3. Please see
[http://www.gnu.org/licenses/gpl-3.0.html](http://www.gnu.org/licenses/gpl-3.0.html) for further information.

Known Issues
------------
On Lion with current Fermi drivers i get an error by the decoder. I think its a driver or OS bug.