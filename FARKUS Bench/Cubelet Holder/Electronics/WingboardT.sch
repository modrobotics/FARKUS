<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="1" fill="9" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="roblocks">
<packages>
<package name="GND_PAD_WING">
<smd name="P$1" x="0" y="0" dx="11" dy="4" layer="1"/>
</package>
<package name="COMM_PIN2">
<smd name="P$1" x="0" y="0" dx="6" dy="6" layer="1" roundness="100"/>
<hole x="0" y="0" drill="2.4"/>
</package>
<package name="VBAT_CIRCLE">
<circle x="0" y="0" radius="4.5" width="0.03" layer="21"/>
<circle x="0" y="0" radius="2" width="0.03" layer="21"/>
<smd name="P$1" x="0" y="3.25" dx="1" dy="2.3" layer="1"/>
<polygon width="0.03" layer="1">
<vertex x="0.1" y="4.5" curve="-90"/>
<vertex x="4.5" y="0.1"/>
<vertex x="4.5" y="0" curve="-90"/>
<vertex x="0" y="-4.5" curve="-90"/>
<vertex x="-4.5" y="0"/>
<vertex x="-4.5" y="0.1" curve="-90"/>
<vertex x="-0.1" y="4.5"/>
<vertex x="-0.1" y="2" curve="90"/>
<vertex x="-2" y="0.1"/>
<vertex x="-2" y="0" curve="90"/>
<vertex x="0" y="-2" curve="90"/>
<vertex x="2" y="0" curve="87.064115"/>
<vertex x="0.1" y="2"/>
</polygon>
<polygon width="0.03" layer="29">
<vertex x="-0.05" y="4.6" curve="90"/>
<vertex x="-4.6" y="0.05"/>
<vertex x="-4.6" y="0" curve="90"/>
<vertex x="0" y="-4.6" curve="90"/>
<vertex x="4.6" y="0"/>
<vertex x="4.6" y="0.05" curve="90"/>
<vertex x="0.05" y="4.6"/>
<vertex x="0.05" y="1.9" curve="-90"/>
<vertex x="1.9" y="0.05"/>
<vertex x="1.9" y="0" curve="-90"/>
<vertex x="0" y="-1.9" curve="-90"/>
<vertex x="-1.9" y="0"/>
<vertex x="-1.9" y="0.05" curve="-90"/>
<vertex x="-0.05" y="1.9"/>
</polygon>
</package>
<package name="PAD2">
<smd name="P$1" x="0" y="0" dx="4" dy="1.5" layer="1"/>
<text x="-1.1" y="1" size="0.5" layer="25">&gt;name</text>
</package>
</packages>
<symbols>
<symbol name="GND_PAD_WING">
<wire x1="0" y1="0" x2="3.81" y2="3.81" width="0.254" layer="94"/>
<wire x1="3.81" y1="3.81" x2="5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="-1.27" x2="5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="1.27" y1="-1.27" x2="0" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="0.8128" layer="95">&gt;name</text>
<pin name="1" x="-2.54" y="0" visible="off" length="short"/>
</symbol>
<symbol name="COMM_PIN2">
<circle x="0" y="0" radius="2.54" width="0.254" layer="94"/>
<circle x="0" y="0" radius="1.27" width="0.254" layer="94"/>
<text x="-3.81" y="3.81" size="1.524" layer="95">&gt;name</text>
<pin name="1" x="5.08" y="0" length="middle" rot="R180"/>
</symbol>
<symbol name="VBAT_CIRCLE">
<circle x="2.54" y="0" radius="3.5921" width="0.254" layer="94"/>
<circle x="2.54" y="0" radius="2.54" width="0.254" layer="94"/>
<text x="2.54" y="5.08" size="1.27" layer="95">&gt;name</text>
<pin name="1" x="-2.54" y="0" visible="pin" length="short"/>
</symbol>
<symbol name="PAD2">
<wire x1="0" y1="2.54" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="7.62" y1="2.54" x2="7.62" y2="-2.54" width="0.254" layer="94"/>
<wire x1="7.62" y1="-2.54" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="2.54" width="0.254" layer="94"/>
<text x="1.016" y="3.302" size="1.27" layer="95">&gt;name</text>
<pin name="1" x="-2.54" y="0" visible="pin" length="short"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND_PAD_WING">
<gates>
<gate name="G$1" symbol="GND_PAD_WING" x="-2.54" y="0"/>
</gates>
<devices>
<device name="" package="GND_PAD_WING">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="COMM_PIN2">
<gates>
<gate name="G$1" symbol="COMM_PIN2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="COMM_PIN2">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="VBAT_CIRCLE">
<gates>
<gate name="G$1" symbol="VBAT_CIRCLE" x="-5.08" y="0"/>
</gates>
<devices>
<device name="" package="VBAT_CIRCLE">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PAD2">
<gates>
<gate name="G$1" symbol="PAD2" x="-5.08" y="0"/>
</gates>
<devices>
<device name="" package="PAD2">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="nev">
<packages>
<package name="SLIDING_CONTACT">
<smd name="P$1" x="-2" y="0" dx="10" dy="2.5" layer="1" rot="R90"/>
<smd name="P$2" x="2" y="0" dx="10" dy="2.5" layer="1" rot="R90"/>
<text x="-3" y="5.5" size="1.27" layer="25">&gt;Name</text>
</package>
</packages>
<symbols>
<symbol name="SLIDING_CONTACT">
<wire x1="-2.54" y1="7.62" x2="-2.54" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-7.62" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="7.62" x2="-2.54" y2="7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="7.62" x2="7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="-7.62" x2="2.54" y2="-7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="2.54" y2="7.62" width="0.254" layer="94"/>
<pin name="P$1" x="-5.08" y="-12.7" length="middle" rot="R90"/>
<pin name="P$2" x="5.08" y="-12.7" length="middle" rot="R90"/>
<wire x1="-10.16" y1="-10.16" x2="10.16" y2="-10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="-10.16" x2="10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="10.16" y1="10.16" x2="-10.16" y2="10.16" width="0.254" layer="94"/>
<wire x1="-10.16" y1="10.16" x2="-10.16" y2="-10.16" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SLIDING_CONTACT">
<gates>
<gate name="G$1" symbol="SLIDING_CONTACT" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SLIDING_CONTACT">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="P2" library="roblocks" deviceset="GND_PAD_WING" device=""/>
<part name="P3" library="roblocks" deviceset="GND_PAD_WING" device=""/>
<part name="P4" library="roblocks" deviceset="GND_PAD_WING" device=""/>
<part name="P5" library="roblocks" deviceset="GND_PAD_WING" device=""/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="GND5" library="supply1" deviceset="GND" device=""/>
<part name="C1" library="roblocks" deviceset="COMM_PIN2" device=""/>
<part name="U$1" library="roblocks" deviceset="VBAT_CIRCLE" device=""/>
<part name="BATT" library="roblocks" deviceset="PAD2" device=""/>
<part name="GND" library="roblocks" deviceset="PAD2" device=""/>
<part name="DAT" library="roblocks" deviceset="PAD2" device=""/>
<part name="J1" library="nev" deviceset="SLIDING_CONTACT" device=""/>
<part name="J2" library="nev" deviceset="SLIDING_CONTACT" device=""/>
<part name="GND2" library="supply1" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<wire x1="86.36" y1="22.225" x2="123.825" y2="22.225" width="0.1524" layer="94"/>
<wire x1="123.825" y1="22.225" x2="123.825" y2="19.05" width="0.1524" layer="94"/>
<wire x1="123.825" y1="19.05" x2="123.825" y2="16.51" width="0.1524" layer="94"/>
<wire x1="123.825" y1="16.51" x2="123.825" y2="13.97" width="0.1524" layer="94"/>
<wire x1="123.825" y1="13.97" x2="123.825" y2="11.43" width="0.1524" layer="94"/>
<wire x1="123.825" y1="11.43" x2="123.825" y2="8.89" width="0.1524" layer="94"/>
<wire x1="123.825" y1="8.89" x2="86.36" y2="8.89" width="0.1524" layer="94"/>
<wire x1="86.36" y1="8.89" x2="86.36" y2="11.43" width="0.1524" layer="94"/>
<wire x1="86.36" y1="11.43" x2="86.36" y2="13.97" width="0.1524" layer="94"/>
<wire x1="86.36" y1="13.97" x2="86.36" y2="16.51" width="0.1524" layer="94"/>
<wire x1="86.36" y1="16.51" x2="86.36" y2="19.05" width="0.1524" layer="94"/>
<wire x1="86.36" y1="19.05" x2="86.36" y2="22.225" width="0.1524" layer="94"/>
<wire x1="86.36" y1="19.05" x2="123.825" y2="19.05" width="0.1524" layer="94"/>
<wire x1="86.36" y1="16.51" x2="123.825" y2="16.51" width="0.1524" layer="94"/>
<wire x1="86.36" y1="13.97" x2="123.825" y2="13.97" width="0.1524" layer="94"/>
<wire x1="86.36" y1="11.43" x2="123.825" y2="11.43" width="0.1524" layer="94"/>
<text x="111.76" y="71.12" size="1.778" layer="91" rot="R180">connector pads for neighbor block</text>
<text x="87.63" y="17.145" size="1.6764" layer="94">COMPANY: Modular Rbotics</text>
<text x="87.63" y="12.065" size="1.6764" layer="94">DESIGNER: Stephane Constantin</text>
<text x="99.06" y="20.32" size="1.778" layer="94">Wingboard A v41</text>
<text x="40.64" y="68.58" size="2.54" layer="94">Wingboard A v41</text>
<text x="87.63" y="14.478" size="1.524" layer="94">PROJECT: Cubelets</text>
<text x="87.63" y="9.398" size="1.524" layer="94">DATE: 6/6/2011</text>
<frame x1="20.32" y1="7.62" x2="124.46" y2="73.66" columns="0" rows="0" layer="94" border-left="no" border-top="no" border-right="no" border-bottom="no"/>
<text x="40.64" y="40.64" size="1.778" layer="91">Sliding contact pads</text>
</plain>
<instances>
<instance part="P2" gate="G$1" x="91.44" y="55.88"/>
<instance part="P3" gate="G$1" x="97.79" y="59.69" rot="R270"/>
<instance part="P4" gate="G$1" x="101.6" y="53.34" rot="R180"/>
<instance part="P5" gate="G$1" x="95.25" y="49.53" rot="R90"/>
<instance part="GND1" gate="1" x="95.25" y="40.64"/>
<instance part="GND5" gate="1" x="39.37" y="53.34"/>
<instance part="C1" gate="G$1" x="115.57" y="52.07" rot="R270"/>
<instance part="U$1" gate="G$1" x="71.12" y="58.42"/>
<instance part="BATT" gate="G$1" x="45.72" y="63.5"/>
<instance part="GND" gate="G$1" x="45.72" y="55.88"/>
<instance part="DAT" gate="G$1" x="45.72" y="48.26"/>
<instance part="J1" gate="G$1" x="38.1" y="27.94"/>
<instance part="J2" gate="G$1" x="66.04" y="27.94"/>
<instance part="GND2" gate="1" x="39.37" y="10.16"/>
</instances>
<busses>
</busses>
<nets>
<net name="VBAT" class="0">
<segment>
<wire x1="30.48" y1="63.5" x2="43.18" y2="63.5" width="0.1524" layer="91"/>
<label x="31.75" y="63.5" size="2.032" layer="95"/>
<pinref part="BATT" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="68.58" y1="63.5" x2="68.58" y2="58.42" width="0.1524" layer="91"/>
<wire x1="68.58" y1="63.5" x2="63.5" y2="63.5" width="0.1524" layer="91"/>
<wire x1="63.5" y1="63.5" x2="63.5" y2="60.96" width="0.1524" layer="91"/>
<wire x1="63.5" y1="60.96" x2="60.96" y2="60.96" width="0.1524" layer="91"/>
<label x="58.42" y="60.96" size="1.778" layer="95"/>
<pinref part="U$1" gate="G$1" pin="1"/>
</segment>
<segment>
<pinref part="J1" gate="G$1" pin="P$1"/>
<wire x1="33.02" y1="15.24" x2="33.02" y2="10.16" width="0.1524" layer="91"/>
<wire x1="33.02" y1="10.16" x2="22.86" y2="10.16" width="0.1524" layer="91"/>
<label x="22.86" y="10.16" size="1.778" layer="95"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="95.25" y1="46.99" x2="95.25" y2="45.72" width="0.1524" layer="91"/>
<wire x1="95.25" y1="45.72" x2="95.25" y2="43.18" width="0.1524" layer="91"/>
<wire x1="88.9" y1="55.88" x2="88.9" y2="45.72" width="0.1524" layer="91"/>
<wire x1="88.9" y1="45.72" x2="95.25" y2="45.72" width="0.1524" layer="91"/>
<wire x1="104.14" y1="53.34" x2="104.14" y2="45.72" width="0.1524" layer="91"/>
<wire x1="104.14" y1="45.72" x2="95.25" y2="45.72" width="0.1524" layer="91"/>
<wire x1="97.79" y1="62.23" x2="104.14" y2="62.23" width="0.1524" layer="91"/>
<wire x1="104.14" y1="62.23" x2="104.14" y2="53.34" width="0.1524" layer="91"/>
<junction x="104.14" y="53.34"/>
<junction x="95.25" y="45.72"/>
<pinref part="P5" gate="G$1" pin="1"/>
<pinref part="GND1" gate="1" pin="GND"/>
<pinref part="P2" gate="G$1" pin="1"/>
<pinref part="P4" gate="G$1" pin="1"/>
<pinref part="P3" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="30.48" y1="55.88" x2="39.37" y2="55.88" width="0.1524" layer="91"/>
<wire x1="30.48" y1="55.88" x2="30.48" y2="58.42" width="0.1524" layer="91"/>
<wire x1="30.48" y1="58.42" x2="43.18" y2="58.42" width="0.1524" layer="91"/>
<wire x1="43.18" y1="58.42" x2="43.18" y2="55.88" width="0.1524" layer="91"/>
<pinref part="GND5" gate="1" pin="GND"/>
<pinref part="GND" gate="G$1" pin="1"/>
</segment>
<segment>
<pinref part="J1" gate="G$1" pin="P$2"/>
<wire x1="39.37" y1="12.7" x2="43.18" y2="12.7" width="0.1524" layer="91"/>
<wire x1="43.18" y1="12.7" x2="43.18" y2="15.24" width="0.1524" layer="91"/>
<pinref part="GND2" gate="1" pin="GND"/>
</segment>
</net>
<net name="DATA" class="0">
<segment>
<wire x1="115.57" y1="46.99" x2="115.57" y2="38.1" width="0.1524" layer="91"/>
<wire x1="115.57" y1="38.1" x2="121.92" y2="38.1" width="0.1524" layer="91"/>
<label x="115.57" y="39.37" size="1.778" layer="95"/>
<pinref part="C1" gate="G$1" pin="1"/>
</segment>
<segment>
<wire x1="30.48" y1="48.26" x2="43.18" y2="48.26" width="0.1524" layer="91"/>
<label x="33.02" y="48.26" size="2.032" layer="95"/>
<pinref part="DAT" gate="G$1" pin="1"/>
</segment>
<segment>
<pinref part="J2" gate="G$1" pin="P$1"/>
<wire x1="60.96" y1="15.24" x2="60.96" y2="10.16" width="0.1524" layer="91"/>
<wire x1="60.96" y1="10.16" x2="50.8" y2="10.16" width="0.1524" layer="91"/>
<label x="50.8" y="10.16" size="1.778" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
