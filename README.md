# SA-MP Masterlist Fix
~~Fixes the infamous SA-MP internet tab bug.~~
Uses an alternative list of servers to replace the now deleted server lists for 0.3.DL which happened a few days after 04 Novemeber 2019.

[4/11](https://forum.sa-mp.com/showthread.php?p=4120379#post4120379)



## Installation:
* Download the latest `version.dll` from the [releases page](https://github.com/OriginalNutter/sa-mp_masterlist_fix/releases);
* Close `samp.exe`
* Copy `version.dll` to SA-MP directory where `samp.exe` is located;
* Run `samp.exe`


## How does it work?
Simple! 

This fix injects a DLL into SA-MP client which redirects all the requests outgoing to SA-MP Masterlist (~~hosted list is not affected~~ The hosted list will be overwritten unless the SA-MP team allows 0.3.DL servers to appear on the hosted tab again) to the server specified in `masterlist_fix.cfg` (default: [samp-servers.net](http://samp-servers.net/))

Why not SACNR Monitor? It does not track 0.3.DL servers lol.

I do not have plans for the hosted tab **just yet** but it might become a list of good servers that make 0.3.DL worthwhile.

## My server still is not appearing on the internet tab.
Your server has most likely not been added to [samp-servers.net](http://samp-servers.net/). 

You can register your server by following submitting your server's IP on the form on [samp-servers.net](http://samp-servers.net/). Your server will then appear on the SA-MP client an hour after it is added to [samp-servers.net](http://samp-servers.net/).


## Contributors:
[spmn](https://github.com/spmn) - Original idea when the internet list went down a few years ago on 0.3.7. <br />
[MyU](https://github.com/myudev) - wrote the code for the auto injector used in v1.1.0 and subsequent releases <br />
[nickdodd25](https://github.com/nickdodd25) - hosted the masterlist for v1.0.1 release
