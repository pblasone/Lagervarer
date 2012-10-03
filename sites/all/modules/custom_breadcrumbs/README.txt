/* $Id $ */

Custom Breadcrumbs 6.x-1.x (See below for 6.x-2.x)
--------------------------------------------------
Summary
-------
* Enable the module
* Assign 'administer custom breadcrumbs' permission to those roles that should
  be allowed to add/edit/delete custom breadcrumbs.
* Assign 'use php in custom breadcrumbs' to roles that should be allowed to use
  php to determine breadcrumb visibility.
* Go to Administer > Site building > Custom breadcrumbs to add new breadcrumbs
* Click "Add a new custom breadcrumb"
* Choose the node type to create a breadcrumb trail for
* For the titles, put each "crumb" one line after another (There is no need to
  put in "home"):

  Item 1
  SubItem A
  SuperSubItem X

* For the paths, put the path to each crumb starting after the domain name. Don't 
  include a leading or trailing slash.

  item1
  item-1/subitem-a
  item-1/subitem-a/supersubitem-x

* Click save to save the breadcrumb
* Visit the page and your breadcrumb should appear!

Description
-----------
As the name suggests, Custom Breadcrumbs allows you to create and modify your
own breadcrumbs based on node type. After enabling the module, click on
Administer > Site building > Custom breadcrumbs. You'll be abel to add new
breadcrumbs from this page.

Clicking on that link, you have the option to select the node type the breadcrumb
will apply to. There are two text fields below-- "Titles" and "Paths." When
creating a breadcrumb, you're simply creating a link. In the custom breadcrumbs
interface "Titles" describes the text of the breadcrumb while "Paths" describes
the Drupal path the breadcrumb links to. Each Title must have a corresponding Path.

To give a very simple example of how to use this module, let's say I have a blog on
my web site called "Deep Thoughts." To create this, I use the Views module to create
a page at /blog that displays all the node types "blog post." Whenever a user views
a blog post I want the breadcrumb to show Home > Deep Thoughts instead of simply
Home. To do this I would simply type "Deep Thoughts" in the "Titles" field and and
"blog" in the "Paths" field and save my breadcrumb.

Using the Tokens module, the Custom breadcrumbs module becomes much more flexible
because breadcrumbs can become dynamic. You can create a breadcrumb like
Home > Deep Thoughts > [Month of Blog Post] [Year of Blog Post], where "Deep Thoughts"
links to my main blog page and "[Month of Blog Post] [Year of Blog Post]" links to
a view that shows only blog posts from the month and year the blog post was created
(e.g. June 2007). For this, you would do the following:

Node Type:
Blog Post

Titles:
Deep Thoughts
[month] [yyyy]

Paths:
blog
blog/[mm]_[yyyy]

(where of course, blog/[mm]_[yyyy] is the path to the view of blog posts from
that month and year). So if you created a blog pos on June 13, 2007 your breadcrumb
would show Home > Deep Thoughts > June 2007 and "June 2007" links to "blog/06_2007"
which is a view of all blog posts from June 2007.

Also, note that Custom Breadcrumbs doesn't actually check to be sure that a
particular path exists, so you'll have to check yourself to avoid 404 errors.

Only users with 'administer custom breadcrumbs' permission will be allowed to
create or modify custom breadcrumbs.

Breadcrumb Visibility
---------------------
Users given 'use php in custom breadcrumbs' permission can include php code snippet
that returns TRUE or FALSE to control whether or not the breadcrumb is displayed. Note
that this code has access to the $node variable, and can check its type or any other
property.

Special Identifiers
-------------------
The following identifiers can be used to achieve a special behavior:
<pathauto> - will clean any path using the current pathauto module settings, if that
             module is installed.
<none>     - can be used as a path to have a breadcrumb element that is not hyperlinked.

Identifiers should be added to the paths area in the following format: identifier|path.
To be recognized, the idenfier must be enclosed in angular brackets, and proceed any
part of the path:

For example: <pathauto>|[ogname-raw]


Custom Breadcrumbs 2.0
----------------------

Summary
-------
* Enable the module and any option submodules (see below for details)
* Assign 'administer custom breadcrumbs' permission to those roles that should
  be allowed to add/edit/delete custom breadcrumbs.
* Assign 'use php in custom breadcrumbs' to roles that should be allowed to use
  php to determine breadcrumb visibility.
* Go to Administer > Site Configuration > Custom breadcrumbs Settings to select
  the 'Home' breacrumb text and possibly other global settings.
* Go to Administer > Site building > Custom breadcrumbs to add new breadcrumbs
* To add a breadcrumb, click on one of the tabs at the top of the page. For example,
  click 'Node' to create a custom breadcrumb based on node type.
* Fill in the required information for the breadcrumb (varies depending on breadcrumb
  type, see below).
* For the titles, put each "crumb" one line after another (There is no need to
  put in "home"):

  Item 1
  SubItem A
  SuperSubItem X

* For the paths, put the path to each crumb starting after the domain name. Don't
  include a leading or trailing slash.

  item1
  item-1/subitem-a
  item-1/subitem-a/supersubitem-x

* Click save to save the breadcrumb
* Visit the page and your breadcrumb should appear!

New Features
------------
In the 6.x-2.x release, custom breadcrumbs has many new features which are available
through optional modules in the custom breadcrumbs package. The base module, required
by all the others, is still custom_breadcrumbs. This module handles custom breadcrumbs
based on node type as described above. The following optional modules can also be
installed to provide custom breadcrumbs in a variety of situations:

+ custom_breadcrumbs_views provides custom breadcrumbs on views pages.
  Once this module is enabled, a new "Views Page" tab
  will appear at admin/build/custom_breadcrumbs. To add a views page breadcrumb, click
  on the tab and then select the view from list of available views. Fill in the
  visibility, title and paths sections as described above, and your breadcrumb should
  appear on your views page. Note that token substitution is possible with global and
  user tokens only. The $view object is available for use in the php_visibility section.

+ custom_breadcrumbs_paths provides custom breadcrumbs on nodes and views at a specified
  path (url). Once this module is enabled, a new "Path" tab will appear at
  admin/build/custom_breadcrumbs.  To add a breadcrumb for a node or a view at a specific
  path, just enter the path in the Specific Path section. Fill in the visibility, title
  and paths sections as described above, and save the breadcrumb. Now your breadcrumb
  should appear on the node/view at the specific path that you selected. Note that custom
  breadcrumbs does not check the validity of the entered path. When entering a path for a
  particular language (see below), do not specify the two-letter language prefix. custom
  breadcrumbs will assume the correct prefix according to the selected language. To use '*'
  as a wildcard, go to custom breadcrumbs configuration page at
  /admin/settings/custom-breadcrumbs and select the 'Use wildcard pattern matching in paths'
  option in the advanced settings section.

+ custom_breadcrumbs_taxonomy provides custom breadcrumbs on taxonomy term pages, views, and
  for nodes that are assigned a taxonomy vocabulary or term. Once this module is enabled,
  two new tabs will appear appear at admin/build/custom_breadcrumbs: Taxonomy Term and
  Taxonomy Vocabulary. Breadcrumb generation can be handled in two different ways.  If
  'use the taxonomy term hierarchy' is checked at custom breadcrumbs configuration page,
  then breadcrumbs will be generated similarly to the taxonomy_breadcrumb module. Otherwise,
  breadcrumb generation will be according to the standard custom_breadcrumb approach.

  In taxonomy breadcrumb mode, the breadcrumb trail is automatically constructed based
  on the taxonomy term hierarchy [HOME] >> [VOCABULARY] >> TERM >> [TERM] >> [TITLE].
  In this mode the breadcrumb titles are the term and vocabulary names. The paths these
  titles are linked to can be assigned via the Taxonomy Term and Taxonomy Vocabulary
  tabs at admin/build/custom_breadcrumbs. Providing a path for a vocabulary will enable
  the [VOCABULARY] portion of the breadcrumb.  The path for a term can similarly be set,
  but if one is not provided the default taxonomy/term/tid (where tid is a number, the
  taxonomy term id) will be used. Select the types of nodes to include or exclude at
  the custom breadcrumbs configuration settings page. The option to add the node title
  at the end of the breadcrumb trail can also be enabled on that page.

  In the standard custom_breadcrumbs mode, you can provid the standard titles and paths
  for constructing the breadcrumb trail on nodes that have defined taxonomy terms. Note
  that if a node has more than one term, the lightest term in the lightest vocabulary with
  a defined custom breadcrumb will be used.

  Note: do not use this module and the taxonomy_breadcrumb module at the same time.
  Custom_breadcrumbs_taxonomy has extended the functionality of taxonomy_breadcrumb, so
  that module is not needed if you are using custom_breadcrumbs.

  While at admin/settings/custom-breadcrumbs, go ahead and enable any additional
  taxonomy breadcrumb options that suits your needs. If you are using views to override
  taxonomy term pages, then be sure to enable the "Use taxonomy breadcrumbs for views?"
  option.

User Interface
--------------
The user interface has been modified for Custom Breadcrumbs 2.0. Breadcrumbs from all
custom breadcrumbs modules are tabulated at admin/build/custom_breadcrumbs. The table
can be sorted according to breadcrumb name, type, language (if locale is enabled) by
clicking on the column headers. The table can also be filtered to display breadcrumbs
of a specific type, language, or combination of the two. A new custom breadcrumbs fieldset
has  been added to node edit pages. All defined breadcrumbs for a particular node are
displayed here, with an option to edit each.  If no breadcrumbs have been defined for a
particular node, then a link can be followed back to admin/build/custom_breacrumbs to add
a custom breadcrumb.

Languages
---------
If the core Locale module is enabled, then an additional option to specify the language
for the breadcrumb is available when constructing the breadcrumb trail (with any of the
custom breadcrumb modules).

HOME breadcrumb
---------------
The text to display at beginning of the breadcrumb trail can be assigned from the custom
breadcrumb configuration settings page. Typically this is home or your site name. You can
Leave it blank to have no home breadcrumb.

Authors
-------
bennybobw, dbabbage, Michelle, MGN
