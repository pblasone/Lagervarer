<?php
// $Id: customer.itpl.php,v 1.5.2.3 2009/02/04 13:44:33 islandusurper Exp $

/**
 * This file is the Lagervarer seller notification template for Ubercart.
 */
?>

<table width="95%" border="0" cellspacing="0" cellpadding="1" align="center" bgcolor="#ffc90a" style="font-family: verdana, arial, helvetica; font-size: small;">
  <tr>
    <td>
      <table width="100%" border="0" cellspacing="0" cellpadding="5" align="center" bgcolor="#FFFFFF" style="font-family: verdana, arial, helvetica; font-size: small;">
        <?php if ($business_header) { ?>
        <tr valign="top">
          <td>
            <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td>
                  <img alt="Lagervarer.dk" src="http://www.lagervarer.dk/sites/all/themes/zenlager/images/logo-small.gif" />
                </td>
                <td width="98%">

                </td>
              </tr>
            </table>

          </td>
        </tr>
        <?php } ?>

        <tr valign="top">
          <td>
                        <br />
            <?php if ($thank_you_message) { ?>
            <p><b><?php echo t("Congratulations, [order-seller-firstname]! You've made a sale!"); ?></b></p>

            <p><b><?php echo t('Please contact the buyer to arrange for payment and delivery.'); ?></b><br />
            <?php echo t("You should contact the buyer as soon as possible to arrange for payment and delivery of the ordered items. You'll find the buyer's contact information below."); ?></p>

            <p><b><?php echo t('Want to manage your sales online?'); ?></b><br />
            <?php echo t('If you need to check the status of your orders, please visit our home page at [store-link] and click on "My account" in the menu or login with the following link:'); ?>
            <br /><br />[site-login]</p>
            <?php } ?>

            <table cellpadding="4" cellspacing="0" border="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
              <tr>
                <td colspan="2" bgcolor="#ffc90a">
                  <b><?php echo t('Buyer Information:'); ?></b>
                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('E-mail Address:'); ?></b>
                </td>
                <td width="98%">
                  [order-email]
                </td>
              </tr>
              <tr>
                <td colspan="2">

                  <table width="100%" cellspacing="0" cellpadding="0" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td valign="top" width="50%">
                        <b><?php echo t('Billing Address:'); ?></b><br />
                        [order-billing-address]<br />
                        <br />
                        <b><?php echo t('Billing Phone:'); ?></b><br />
                        [order-billing-phone]<br />
                      </td>
                      <?php if (uc_order_is_shippable($order)) { ?>
                      <td valign="top" width="50%">
                        <b><?php echo t('Shipping Address:'); ?></b><br />
                        [order-shipping-address]<br />
                        <br />
                        <b><?php echo t('Shipping Phone:'); ?></b><br />
                        [order-shipping-phone]<br />
                      </td>
                      <?php } ?>
                    </tr>
                  </table>

                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Order Grand Total:'); ?></b>
                </td>
                <td width="98%">
                  <b>[order-total]</b>
                </td>
              </tr>
              <tr>
                <td nowrap="nowrap">
                  <b><?php echo t('Payment Method:'); ?></b>
                </td>
                <td width="98%">
                  [order-payment-method]
                </td>
              </tr>

              <tr>
                <td colspan="2" bgcolor="#ffc90a">
                  <b><?php echo t('Order Summary:'); ?></b>
                </td>
              </tr>

              <?php if (uc_order_is_shippable($order)) { ?>
              <tr>
                <td colspan="2" bgcolor="#EEEEEE">
                  <font color="#CC6600"><b><?php echo t('Shipping Details:'); ?></b></font>
                </td>
              </tr>
              <?php } ?>

              <tr>
                <td colspan="2">

                  <table border="0" cellpadding="1" cellspacing="0" width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Order #:'); ?></b>
                      </td>
                      <td width="98%">
                        [order-link]
                      </td>
                    </tr>

                    <?php if ($shipping_method && uc_order_is_shippable($order)) { ?>
                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Shipping Method:'); ?></b>
                      </td>
                      <td width="98%">
                        [order-shipping-method]
                      </td>
                    </tr>
                    <?php } ?>

                    <tr>
                      <td nowrap="nowrap">
                        <?php echo t('Products Subtotal:'); ?>&nbsp;
                      </td>
                      <td width="98%">
                        [order-subtotal]
                      </td>
                    </tr>

                    <?php foreach ($line_items as $item) {
                    if ($item['line_item_id'] == 'subtotal' || $item['line_item_id'] == 'total') {
                      continue;
                    }?>

                    <tr>
                      <td nowrap="nowrap">
                        <?php echo $item['title']; ?>:
                      </td>
                      <td>
                        <?php echo uc_currency_format($item['amount']); ?>
                      </td>
                    </tr>

                    <?php } ?>

                    <tr>
                      <td>&nbsp;</td>
                      <td>------</td>
                    </tr>

                    <tr>
                      <td nowrap="nowrap">
                        <b><?php echo t('Total for this Order:'); ?>&nbsp;</b>
                      </td>
                      <td>
                        <b>[order-total]</b>
                      </td>
                    </tr>

                    <tr>
                      <td colspan="2">
                        <br /><br /><b><?php echo t('Products on order:'); ?>&nbsp;</b>

                        <table width="100%" style="font-family: verdana, arial, helvetica; font-size: small;">

                          <?php if (is_array($order->products)) {
                            foreach ($order->products as $product) { ?>
                          <tr>
                            <td valign="top" nowrap="nowrap">
                              <b><?php echo $product->qty; ?> x </b>
                            </td>
                            <td width="98%">
                              <b><?php echo $product->title .' - '. uc_currency_format($product->price * $product->qty); ?></b>
                              <?php if ($product->qty > 1) {
                                echo t('(!price each)', array('!price' => uc_currency_format($product->price)));
                              } ?>
                              <br />
                              <?php if (is_array($product->data['attributes']) && count($product->data['attributes']) > 0) {?>
                              <?php foreach ($product->data['attributes'] as $key => $value) {
                                echo '<li>'. $key .': '. $value .'</li>';
                              } ?>
                              <?php } ?>
                              <br />
                            </td>
                          </tr>
                          <?php }
                              }?>
                        </table>

                      </td>
                    </tr>
                  </table>

                </td>
              </tr>

              <?php if ($help_text || $email_text || $store_footer) { ?>
              <tr>
                <td colspan="2">
                  <hr noshade="noshade" size="1" /><br />

                  <?php if ($email_text) { ?>
                  <p><?php echo t('Please note: This e-mail message is an automated notification. Please do not reply to this message.'); ?></p>

                  <p><?php echo t('Thanks again for selling through us.'); ?></p>
                  <?php } ?>

                  <?php if ($store_footer) { ?>
                  <p><b>[store-link]</b><br /><b>[site-slogan]</b></p>
                  <?php } ?>
                </td>
              </tr>
              <?php } ?>

            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
