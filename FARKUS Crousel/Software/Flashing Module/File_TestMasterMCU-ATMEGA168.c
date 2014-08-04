<!DOCTYPE html>
<html lang="en" dir="ltr" class="client-nojs">
<head>
<title>File:TestMasterMCU-ATMEGA168.c - Modular Robotics FARKUS</title>
<meta charset="UTF-8" />
<meta name="generator" content="MediaWiki 1.21.1" />
<link rel="shortcut icon" href="/favicon.ico" />
<link rel="search" type="application/opensearchdescription+xml" href="/farkus/opensearch_desc.php" title="Modular Robotics FARKUS (en)" />
<link rel="EditURI" type="application/rsd+xml" href="http://www.modrobotics.com/farkus/api.php?action=rsd" />
<link rel="alternate" type="application/atom+xml" title="Modular Robotics FARKUS Atom feed" href="/farkus/index.php?title=Special:RecentChanges&amp;feed=atom" />
<link rel="stylesheet" href="http://www.modrobotics.com/farkus/load.php?debug=false&amp;lang=en&amp;modules=ext.rtlcite%7Cmediawiki.legacy.commonPrint%2Cshared%7Cskins.vector&amp;only=styles&amp;skin=vector&amp;*" />
<meta name="ResourceLoaderDynamicStyles" content="" />
<style>a:lang(ar),a:lang(ckb),a:lang(fa),a:lang(kk-arab),a:lang(mzn),a:lang(ps),a:lang(ur){text-decoration:none}
/* cache key: MR_Farkus_Wiki:resourceloader:filter:minify-css:7:2fd99ed5d7a852ee546e986a71379042 */</style>

<script src="http://www.modrobotics.com/farkus/load.php?debug=false&amp;lang=en&amp;modules=startup&amp;only=scripts&amp;skin=vector&amp;*"></script>
<script>if(window.mw){
mw.config.set({"wgCanonicalNamespace":"File","wgCanonicalSpecialPageName":false,"wgNamespaceNumber":6,"wgPageName":"File:TestMasterMCU-ATMEGA168.c","wgTitle":"TestMasterMCU-ATMEGA168.c","wgCurRevisionId":65,"wgArticleId":23,"wgIsArticle":true,"wgAction":"view","wgUserName":null,"wgUserGroups":["*"],"wgCategories":[],"wgBreakFrames":false,"wgPageContentLanguage":"en","wgSeparatorTransformTable":["",""],"wgDigitTransformTable":["",""],"wgDefaultDateFormat":"dmy","wgMonthNames":["","January","February","March","April","May","June","July","August","September","October","November","December"],"wgMonthNamesShort":["","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"],"wgRelevantPageName":"File:TestMasterMCU-ATMEGA168.c","wgRestrictionEdit":[],"wgRestrictionMove":[],"wgRestrictionUpload":[]});
}</script><script>if(window.mw){
mw.loader.implement("user.options",function(){mw.user.options.set({"ccmeonemails":0,"cols":80,"date":"default","diffonly":0,"disablemail":0,"disablesuggest":0,"editfont":"default","editondblclick":0,"editsection":1,"editsectiononrightclick":0,"enotifminoredits":0,"enotifrevealaddr":0,"enotifusertalkpages":1,"enotifwatchlistpages":0,"extendwatchlist":0,"externaldiff":0,"externaleditor":0,"fancysig":0,"forceeditsummary":0,"gender":"unknown","hideminor":0,"hidepatrolled":0,"imagesize":2,"justify":0,"math":1,"minordefault":0,"newpageshidepatrolled":0,"nocache":0,"noconvertlink":0,"norollbackdiff":0,"numberheadings":0,"previewonfirst":0,"previewontop":1,"quickbar":5,"rcdays":7,"rclimit":50,"rememberpassword":0,"rows":25,"searchlimit":20,"showhiddencats":0,"showjumplinks":1,"shownumberswatching":1,"showtoc":1,"showtoolbar":1,"skin":"vector","stubthreshold":0,"thumbsize":2,"underline":2,"uselivepreview":0,"usenewrc":0,"watchcreations":0,"watchdefault":0,"watchdeletion":0,"watchlistdays":3,
"watchlisthideanons":0,"watchlisthidebots":0,"watchlisthideliu":0,"watchlisthideminor":0,"watchlisthideown":0,"watchlisthidepatrolled":0,"watchmoves":0,"wllimit":250,"variant":"en","language":"en","searchNs0":true,"searchNs1":false,"searchNs2":false,"searchNs3":false,"searchNs4":false,"searchNs5":false,"searchNs6":false,"searchNs7":false,"searchNs8":false,"searchNs9":false,"searchNs10":false,"searchNs11":false,"searchNs12":false,"searchNs13":false,"searchNs14":false,"searchNs15":false,"searchNs274":false,"searchNs275":false});;},{},{});mw.loader.implement("user.tokens",function(){mw.user.tokens.set({"editToken":"+\\","patrolToken":false,"watchToken":false});;},{},{});
/* cache key: MR_Farkus_Wiki:resourceloader:filter:minify-js:7:8a8962d7449471ca36ccaede53fed188 */
}</script>
<script>if(window.mw){
mw.loader.load(["mediawiki.page.startup","mediawiki.legacy.wikibits","mediawiki.legacy.ajax"]);
}</script>
<!--[if lt IE 7]><style type="text/css">body{behavior:url("/farkus/skins/vector/csshover.min.htc")}</style><![endif]--></head>
<body class="mediawiki ltr sitedir-ltr ns-6 ns-subject page-File_TestMasterMCU-ATMEGA168_c skin-vector action-view vector-animateLayout">
		<div id="mw-page-base" class="noprint"></div>
		<div id="mw-head-base" class="noprint"></div>
		<!-- content -->
		<div id="content" class="mw-body" role="main">
			<a id="top"></a>
			<div id="mw-js-message" style="display:none;"></div>
						<!-- firstHeading -->
			<h1 id="firstHeading" class="firstHeading" lang="en"><span dir="auto">File:TestMasterMCU-ATMEGA168.c</span></h1>
			<!-- /firstHeading -->
			<!-- bodyContent -->
			<div id="bodyContent">
								<!-- tagline -->
				<div id="siteSub">From Modular Robotics FARKUS</div>
				<!-- /tagline -->
								<!-- subtitle -->
				<div id="contentSub"></div>
				<!-- /subtitle -->
																<!-- jumpto -->
				<div id="jump-to-nav" class="mw-jump">
					Jump to:					<a href="#mw-navigation">navigation</a>, 					<a href="#p-search">search</a>
				</div>
				<!-- /jumpto -->
								<!-- bodycontent -->
				<div id="mw-content-text"><ul id="filetoc"><li><a href="#file">File</a></li>
<li><a href="#filehistory">File history</a></li>
<li><a href="#filelinks">File usage</a></li></ul><div class="fullMedia"><span class="dangerousLink"><a href="/farkus/images/f/f6/TestMasterMCU-ATMEGA168.c" class="internal" title="TestMasterMCU-ATMEGA168.c">TestMasterMCU-ATMEGA168.c</a></span> &#8206;<span class="fileInfo">(file size: 17 KB, MIME type: text/x-c)</span></div>
<div class="mediaWarning"><b>Warning:</b> This file type may contain malicious code.
By executing it, your system may be compromised.</div>
<div id="mw-imagepage-content" lang="en" dir="ltr" class="mw-content-ltr">
<!-- 
NewPP limit report
Preprocessor visited node count: 0/1000000
Preprocessor generated node count: 2/1000000
Post‐expand include size: 0/2097152 bytes
Template argument size: 0/2097152 bytes
Highest expansion depth: 0/40
Expensive parser function count: 0/100
-->

<!-- Saved in parser cache with key MR_Farkus_Wiki:pcache:idhash:23-0!*!*!*!*!*!* and timestamp 20140804231425 -->
</div><h2 id="filehistory">File history</h2>
<div id="mw-imagepage-section-filehistory">
<p>Click on a date/time to view the file as it appeared at that time.
</p>
<table class="wikitable filehistory">
<tr><td></td><th>Date/Time</th><th>Dimensions</th><th>User</th><th>Comment</th></tr>
<tr><td>current</td><td class='filehistory-selected' style='white-space: nowrap;'><a href="/farkus/images/f/f6/TestMasterMCU-ATMEGA168.c">17:29, 25 July 2013</a></td><td> <span style="white-space: nowrap;">(17 KB)</span></td><td><a href="/farkus/index.php?title=User:Jmoyes&amp;action=edit&amp;redlink=1" class="new mw-userlink" title="User:Jmoyes (page does not exist)">Jmoyes</a> <span style="white-space: nowrap;"> <span class="mw-usertoollinks">(<a href="/farkus/index.php?title=User_talk:Jmoyes&amp;action=edit&amp;redlink=1" class="new" title="User talk:Jmoyes (page does not exist)">Talk</a> | <a href="/farkus/index.php/Special:Contributions/Jmoyes" title="Special:Contributions/Jmoyes">contribs</a>)</span></span></td><td dir="ltr"></td></tr>
</table>

</div>
<ul>
<li id="mw-imagepage-upload-disallowed">You cannot overwrite this file.</li>
<li id="mw-imagepage-edit-external"><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;action=edit&amp;externaledit=true&amp;mode=file" title="File:TestMasterMCU-ATMEGA168.c">Edit this file using an external application</a> <small>(See the <a rel="nofollow" class="external text" href="//www.mediawiki.org/wiki/Manual:External_editors">setup instructions</a> for more information)</small></li>
</ul>
<h2 id="filelinks">File usage</h2>
<div id='mw-imagepage-section-linkstoimage'>
<p>The following page links to this file:
</p><ul class="mw-imagepage-linkstoimage">
<li class="mw-imagepage-linkstoimage-ns0"><a href="/farkus/index.php/Crousel" title="Crousel">Crousel</a></li>
</ul>
</div>
</div>				<!-- /bodycontent -->
								<!-- printfooter -->
				<div class="printfooter">
				Retrieved from "<a href="http://www.modrobotics.com/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;oldid=65">http://www.modrobotics.com/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;oldid=65</a>"				</div>
				<!-- /printfooter -->
												<!-- catlinks -->
				<div id='catlinks' class='catlinks catlinks-allhidden'></div>				<!-- /catlinks -->
												<div class="visualClear"></div>
				<!-- debughtml -->
								<!-- /debughtml -->
			</div>
			<!-- /bodyContent -->
		</div>
		<!-- /content -->
		<div id="mw-navigation">
			<h2>Navigation menu</h2>
			<!-- header -->
			<div id="mw-head">
				
<!-- 0 -->
<div id="p-personal" role="navigation" class="">
	<h3>Personal tools</h3>
	<ul>
<li id="pt-login"><a href="/farkus/index.php?title=Special:UserLogin&amp;returnto=File%3ATestMasterMCU-ATMEGA168.c" title="You are encouraged to log in; however, it is not mandatory [o]" accesskey="o">Log in</a></li>	</ul>
</div>

<!-- /0 -->
				<div id="left-navigation">
					
<!-- 0 -->
<div id="p-namespaces" role="navigation" class="vectorTabs">
	<h3>Namespaces</h3>
	<ul>
					<li  id="ca-nstab-image" class="selected"><span><a href="/farkus/index.php/File:TestMasterMCU-ATMEGA168.c"  title="View the file page [c]" accesskey="c">File</a></span></li>
					<li  id="ca-talk" class="new"><span><a href="/farkus/index.php?title=File_talk:TestMasterMCU-ATMEGA168.c&amp;action=edit&amp;redlink=1"  title="Discussion about the content page [t]" accesskey="t">Discussion</a></span></li>
			</ul>
</div>

<!-- /0 -->

<!-- 1 -->
<div id="p-variants" role="navigation" class="vectorMenu emptyPortlet">
	<h3 id="mw-vector-current-variant">
		</h3>
	<h3><span>Variants</span><a href="#"></a></h3>
	<div class="menu">
		<ul>
					</ul>
	</div>
</div>

<!-- /1 -->
				</div>
				<div id="right-navigation">
					
<!-- 0 -->
<div id="p-views" role="navigation" class="vectorTabs">
	<h3>Views</h3>
	<ul>
					<li id="ca-view" class="selected"><span><a href="/farkus/index.php/File:TestMasterMCU-ATMEGA168.c" >Read</a></span></li>
					<li id="ca-viewsource"><span><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;action=edit"  title="This page is protected.&#10;You can view its source [e]" accesskey="e">View source</a></span></li>
					<li id="ca-history" class="collapsible"><span><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;action=history"  title="Past revisions of this page [h]" accesskey="h">View history</a></span></li>
			</ul>
</div>

<!-- /0 -->

<!-- 1 -->
<div id="p-cactions" role="navigation" class="vectorMenu emptyPortlet">
	<h3><span>Actions</span><a href="#"></a></h3>
	<div class="menu">
		<ul>
					</ul>
	</div>
</div>

<!-- /1 -->

<!-- 2 -->
<div id="p-search" role="search">
	<h3><label for="searchInput">Search</label></h3>
	<form action="/farkus/index.php" id="searchform">
				<div>
			<input type="search" name="search" title="Search Modular Robotics FARKUS [f]" accesskey="f" id="searchInput" />			<input type="submit" name="go" value="Go" title="Go to a page with this exact name if exists" id="searchGoButton" class="searchButton" />			<input type="submit" name="fulltext" value="Search" title="Search the pages for this text" id="mw-searchButton" class="searchButton" />					<input type='hidden' name="title" value="Special:Search"/>
		</div>
	</form>
</div>

<!-- /2 -->
				</div>
			</div>
			<!-- /header -->
			<!-- panel -->
			<div id="mw-panel">
				<!-- logo -->
					<div id="p-logo" role="banner"><a style="background-image: url(/farkus/images/farkus_logo.png);" href="/farkus/index.php/Main_Page"  title="Visit the main page"></a></div>
				<!-- /logo -->
				
<!-- Movers -->
<div class="portal" role="navigation" id='p-Movers'>
	<h3>Movers</h3>
	<div class="body">
		<ul>
			<li id="n---Crousel"><a href="/farkus/index.php/Crousel">- Crousel</a></li>
			<li id="n---Conveyance"><a href="/farkus/index.php/Conveyance">- Conveyance</a></li>
		</ul>
	</div>
</div>

<!-- /Movers -->

<!-- Modules -->
<div class="portal" role="navigation" id='p-Modules'>
	<h3>Modules</h3>
	<div class="body">
		<ul>
			<li id="n---Lightie-tester"><a href="/farkus/index.php/Lightie_tester">- Lightie tester</a></li>
			<li id="n---Communication-Tester"><a href="/farkus/index.php/Cubelets_communication_jig">- Communication Tester</a></li>
			<li id="n---Ring-Stamper"><a href="/farkus/index.php/Ring_Stamper">- Ring Stamper</a></li>
		</ul>
	</div>
</div>

<!-- /Modules -->

<!-- SEARCH -->

<!-- /SEARCH -->

<!-- TOOLBOX -->
<div class="portal" role="navigation" id='p-tb'>
	<h3>Toolbox</h3>
	<div class="body">
		<ul>
			<li id="t-whatlinkshere"><a href="/farkus/index.php/Special:WhatLinksHere/File:TestMasterMCU-ATMEGA168.c" title="A list of all wiki pages that link here [j]" accesskey="j">What links here</a></li>
			<li id="t-recentchangeslinked"><a href="/farkus/index.php/Special:RecentChangesLinked/File:TestMasterMCU-ATMEGA168.c" title="Recent changes in pages linked from this page [k]" accesskey="k">Related changes</a></li>
			<li id="t-specialpages"><a href="/farkus/index.php/Special:SpecialPages" title="A list of all special pages [q]" accesskey="q">Special pages</a></li>
			<li id="t-print"><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;printable=yes" rel="alternate" title="Printable version of this page [p]" accesskey="p">Printable version</a></li>
			<li id="t-permalink"><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;oldid=65" title="Permanent link to this revision of the page">Permanent link</a></li>
			<li id="t-info"><a href="/farkus/index.php?title=File:TestMasterMCU-ATMEGA168.c&amp;action=info">Page information</a></li>
		</ul>
	</div>
</div>

<!-- /TOOLBOX -->

<!-- LANGUAGES -->

<!-- /LANGUAGES -->
			</div>
			<!-- /panel -->
		</div>
		<!-- footer -->
		<div id="footer" role="contentinfo">
							<ul id="footer-info">
											<li id="footer-info-lastmod"> This page was last modified on 25 July 2013, at 17:29.</li>
											<li id="footer-info-viewcount">This page has been accessed 9 times.</li>
									</ul>
							<ul id="footer-places">
											<li id="footer-places-privacy"><a href="/farkus/index.php/FARKUS:Privacy_policy" title="FARKUS:Privacy policy">Privacy policy</a></li>
											<li id="footer-places-about"><a href="/farkus/index.php/FARKUS:About" title="FARKUS:About">About Modular Robotics FARKUS</a></li>
											<li id="footer-places-disclaimer"><a href="/farkus/index.php/FARKUS:General_disclaimer" title="FARKUS:General disclaimer">Disclaimers</a></li>
									</ul>
										<ul id="footer-icons" class="noprint">
					<li id="footer-poweredbyico">
						<a href="//www.mediawiki.org/"><img src="/farkus/skins/common/images/poweredby_mediawiki_88x31.png" alt="Powered by MediaWiki" width="88" height="31" /></a>
					</li>
				</ul>
						<div style="clear:both"></div>
		</div>
		<!-- /footer -->
		<script>if(window.mw){
mw.loader.state({"site":"loading","user":"missing","user.groups":"ready"});
}</script>
<script>if(window.mw){
mw.loader.load(["mediawiki.action.view.postEdit","mediawiki.user","mediawiki.page.ready","mediawiki.searchSuggest","mediawiki.hidpi","skins.vector.js"], null, true);
}</script>
<script>if(window.mw){
mw.loader.state({"site":"ready"});
}</script>
<!-- Served in 0.588 secs. -->
	</body>
</html>
