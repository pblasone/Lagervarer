<?php
/**
 * This file is the Lagervarer e-mail notification for saved searches.
 */
?>

<HTML><HEAD>
<META http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<STYLE type=text/css>
<!--
.text {  font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 11px}
a:link {  color: #006699; text-decoration: underline}
a:visited {  color: #006699; text-decoration: underline}
a:hover {  color: #006699; text-decoration: underline}
-->
</STYLE>

</HEAD>
<BODY text=#000000 bgColor=#e0ebf5>
<TABLE cellSpacing=2 cellPadding=0 width=600 align=center border=0>
<TBODY>
<TR>
<TD bgColor=#ffffff>
<TABLE height=80 cellSpacing=0 cellPadding=20 width=600 border=0>
<TBODY>
<TR>
<TD><a href="http://www.lagervarer.dk"><IMG alt=Lagervarer.dk src="http://www.lagervarer.dk/sites/all/themes/zenlager/images/logo-small.gif"></a></TD></TR></TBODY></TABLE></TD></TR>
<TR>
<TD class=text bgColor=#ffffff>
<TABLE class=text cellSpacing=1 cellPadding=2 width="100%" border=0>
<TBODY>
<TR bgColor=#e0ebf5>
<TD bgColor=#e0ebf5>
<DIV align=center><B><?php print t('Search agent'); ?></B></DIV>
</TD></TR></TBODY></TABLE></TD></TR>
<TR>
<TD bgColor=#ffffff>
<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
<TBODY>
<TR>
<TD class=text><FONT size=2><B><?php print t('New items match your search'); ?></B></FONT><BR><BR><?php print t("Your search agent <b>[lv-search-name]</b> found <b>[lv-search-count]</b> new items that match your criteria. This list shows the latest [lv-search-count-preview] matching items."); ?><BR><BR>
<?php print t("Click on an item to go directly to the product detail page, or click on the link below to view the full search results."); ?>

<BR><BR><a href="[lv-search-url]"><?php print t('Show all <b>[lv-search-count]</b> new products that match your search'); ?></a><BR><BR>
[[PRODUCTTABLE]]
<BR><a href="[lv-search-url]"><?php print t('Show all <b>[lv-search-count]</b> new products that match your search'); ?></a>
</TD></TR></TBODY></TABLE></TD></TR>
<TR>
<TD bgColor=#ffffff>
<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
<TBODY>
<TR>
<TD class=text bgColor=#eff5fa><FONT face="Verdana, Arial, Helvetica, sans-serif" size=1><?php print t('You are receiving this e-mail, because you set up an e-mail search agent that will notify you according to your pre-selected interval, whenever new products match your search criteria.'); ?> <A href="[lv-search-url]"><?php print t('Click here to edit the settings for this search agent or to disable or delete it'); ?></A>.</FONT></TD></TR></TBODY></TABLE></TD></TR>
<TR>
<TD bgColor=#eff5fa height=18>
<DIV align=center><FONT face="Verdana, Arial, Helvetica, sans-serif" size=1>Copyright &copy; 2009 Lagervarer.dk.</FONT></DIV></TD></TR>



</TBODY></TABLE></BODY></HTML>