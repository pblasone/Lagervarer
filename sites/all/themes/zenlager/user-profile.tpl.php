<?php
// $Id: user-profile.tpl.php,v 1.2.2.1 2008/10/15 13:52:04 dries Exp $

/**
 * @file user-profile.tpl.php
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * By default, all user profile data is printed out with the $user_profile
 * variable. If there is a need to break it up you can use $profile instead.
 * It is keyed to the name of each category or other data attached to the
 * account. If it is a category it will contain all the profile items. By
 * default $profile['summary'] is provided which contains data on the user's
 * history. Other data can be included by modules. $profile['user_picture'] is
 * available by default showing the account picture.
 *
 * Also keep in mind that profile items and their categories can be defined by
 * site administrators. They are also available within $profile. For example,
 * if a site is configured with a category of "contact" with
 * fields for of addresses, phone numbers and other related info, then doing a
 * straight print of $profile['contact'] will output everything in the
 * category. This is useful for altering source order and adding custom
 * markup for the group.
 *
 * To check for all available data within $profile, use the code below.
 *
 * @code
 *   print '<pre>'. check_plain(print_r($profile, 1)) .'</pre>';
 * @endcode
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-field.tpl.php
 *   Where the html is handled for each item in the group.
 *
 * Available variables:
 *   - $user_profile: All user profile data. Ready for print.
 *   - $profile: Keyed array of profile categories and their items or other data
 *     provided by modules.
 *
 * @see template_preprocess_user_profile()
 */
 global $user;
?>
<div class="profile">

	<p>
		<?php print t('Welcome to your personal user account on @sitename. Here you can get an overview of your sale items, information about purchases and sales, and you can edit your public profile, which will be displayed to other users.', array('@sitename' => variable_get('site-name', 'Lagervarer.dk'))); ?>
	</p>

<?php if (in_array('handlende', array_values($user->roles))): ?>	
	<h2><?php print t('How to get started'); ?></h2>
	<div class="lv-box">
		<div class="usertype-image"><img src="/sites/all/themes/zenlager/images/adviceBulklots.png" /></div>
		<div class="usertype-content">
			<h3><?php print t('Register your bulk lots'); ?></h3>
			<p>
			   <?php print t("You can register as many bulk lots as you desire. It's completely free to register and sell. When registering a bulk lot, you can choose to sell it at a fixed price, allow users to bargain for it, or you can put it on auction. To register your first lot click <strong>Add new bulk lot</strong> in the left menu or click on the button below."); ?>
			</p>	
			<div class="usertype-button">
				<a class="linkbutton" href="<?php print url('node/add/vareparti') ?>"><?php print t('Register a bulk lot'); ?></a>
			</div>							
		</div>
			<div style="clear: both;"></div>		
    </div>
	
		<div class="lv-box">
		<div class="usertype-image"><img src="/sites/all/themes/zenlager/images/adviceRetail.png" /></div>
		<div class="usertype-content">
			<h3><?php print t('Sell single items to consumers'); ?></h3>
			<p>
			   <?php print t('@sitename lets you sell the items in your bulk lots or other single items to consumers, just like a normal webshop. You can put as many items for sale as you desire. Single items can be registered directly by clicking the <strong>Add new retail item</strong> in the menu, or if they are part of a bulk lot, directly in the bulk lot creation form.', array('@sitename' => variable_get('site-name', 'Lagervarer.dk'))); ?>
			</p>
			<div class="usertype-button">
				<a class="linkbutton" href="<?php print url('node/add/consumer') ?>"><?php print t('Register a retail item'); ?></a>
			</div>								
		</div>
			<div style="clear: both;"></div>		
    </div>
	
		<div class="lv-box">
		<div class="usertype-image"><img src="/sites/all/themes/zenlager/images/adviceProfile.png" /></div>
		<div class="usertype-content">
			<h3><?php print t('Update your profile information'); ?></h3>
			<p>
			   <?php print t("@sitename is not only a trading platform, it's also a community complete with discussion forums, groups, blogs, etc. where you can make new contacts among other professionals. Updating your profile with a picture and a brief description of yourself or your company is the first step towards making new contacts through @sitename. Edit your profile information by clicking <strong>Edit details</strong> in the menu and selecting the <strong>Profile</strong> tab, or click the button below.", array('@sitename' => variable_get('site-name', 'Lagervarer.dk'))); ?>
			</p>			
			<div class="usertype-button">
				<a class="linkbutton" href="<?php print url('user/' . $user->uid . '/edit/profile') ?>"><?php print t('Edit your profile information'); ?></a>
			</div>								
		</div>
			<div style="clear: both;"></div>		
    </div>
<?php endif; ?>

  <?php print $profile['summary']; ?>
</div>
