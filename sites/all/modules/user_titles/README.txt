When installing this module, it won't appear to do anything at first. This is
because you need to modify your theme in order to place the output where
you like.

The function you need to call to get the data is
user_titles_get_user_title(). This module will automatically populate the
$user_title and $user_title_image variables into your nodes and comments.

In your node.tpl.php and/or your comment.tpl.php, choose where you would like
the user title to appear, and place this code:

  <?php if ($user_title): ?>
    <div class="user-title">
      <?php
        print $user_title;
        print $user_title_image ? $user_title_image : '';
      ?>
    </div>
  <?php endif; ?>
