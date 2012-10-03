<?php
/**
 * This file is the Lagervarer e-mail notification for expiring products.
 */
?>

<HTML><HEAD>
<META http-equiv=Content-Type content="text/html; charset=iso-8859-1">
<STYLE type=text/css>
<!--
.text {  font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12px}
h2 { font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 16px}
h3 { font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 14px}
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
<DIV align=center><B><?php print t('Reminder'); ?></B></DIV>
</TD></TR></TBODY></TABLE></TD></TR>
<TR>
<TD bgColor=#ffffff>
<TABLE cellSpacing=0 cellPadding=20 width="100%" border=0>
<TBODY>
<TR>
<TD class=text><FONT size=2><B><?php print t('Please renew your listings on Lagervarer.dk.'); ?></B></FONT><BR><BR><?php print t("In order for us to ensure our users, that the listings they find on Lagervarer.dk are current and that listed lots are still available, we ask all our sellers to update each listing at least once a month."); ?><BR><BR>
<?php print t("Below is a list of expiring or already expired items. Please follow these links to quickly verify your listed items, and update the ones that are still for sale."); ?>

[[LINKS]]
[[PRODUCTTABLE]]

</TD></TR></TBODY></TABLE></TD></TR>

<TR>
<TD bgColor=#eff5fa height=18>
<DIV align=center><FONT face="Verdana, Arial, Helvetica, sans-serif" size=1>Copyright &copy; 2011 Lagervarer.dk.</FONT></DIV></TD></TR>



</TBODY></TABLE></BODY></HTML>