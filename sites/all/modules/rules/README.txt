$Id: README.txt,v 1.1.2.3 2008/08/21 13:35:47 fago Exp $

Rules Module
------------
by Wolfgang Ziegler, nuppla@zites.net


The rules modules allows site administrators to define conditionally executed actions
 based on occurring events (ECA-rules). It's a replacement with more features for the
 trigger module in core and the successor of the workflow-ng module.

It opens new opportunities for site builders to extend the site in ways not possible
before.



Installation
-------------

*Before* starting, make sure that you have read at least the introduction - so you know 
at least the basic concepts. You can find it here:
                     
                          http://drupal.org/node/156288

 * Copy the whole rules directory to your modules directory and
   activate the rules module.
 * You can find the admin interface at /admin/rules.

Notes:

 * If you have the php module activated, you can use a php input evaluator in 
   your rules.

 * If you install the token module, you can make use of token replacements in your rules.
   Get the module from http://drupal.org/project/token and activate it - that's it.

   You don't need to enable the token actions module as rules provides already equivalent
   actions, which are better integrated into the rules module. 


Rules Scheduler
---------------

 * If you enable the rules scheduler module, you get new actions, that allow you to
   schedule the execution of rule sets.
 * Make sure that you have configured cron for your drupal installation as cron
   is used for scheduling the rule sets. For help see http://drupal.org/cron


Rules Simpletest
----------------

 This module just provides some test case for the rules module. You don't need to
 activate it unless you want to run these tests with the simpletest module.

