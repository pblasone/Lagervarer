<?php
// $Id: uc_auction.user.inc,v 1.1.2.20 2009/03/20 17:00:16 garrettalbright Exp $

/**
 * @file
 * Contains callbacks for user features which don't have to be placed in the
 * main .module file.
 *
 * @ingroup uc_auction
 */

/**
 * Show a table of auctions the user has bid upon.
 *
 * @param $user
 *   The user in question.
 * @param $type
 *   The type of auctions to display - 'active', 'lost' or 'won'.
 * @return
 *   A themed table of auctions.
 */

function uc_auction_user_auctions($user, $type) {
	$time = time();
	$rows = array();
	if ($type === 'active') {
		// Show active (non-expired) auctions the user has bid on.
		drupal_add_css(drupal_get_path('module', 'uc_auction') . '/uc_auction.css', 'module');
		$headers = array(t('Item'), t('High bid'), t('Bids'), t('Expires in'), t('High bidder?'));
		$rez = pager_query('SELECT DISTINCT uab.nid FROM {uc_auction_bids} uab INNER JOIN {uc_auction} ua ON uab.nid = ua.nid WHERE uab.uid = %d && ua.expiry >= %d ORDER BY ua.expiry DESC', 20, 0, NULL, $user->uid, $time);
		while ($nid = db_fetch_object($rez)) {
			$node = node_load($nid->nid);
			$rows[] = array(
				l($node->title, "node/{$node->nid}"),
				uc_currency_format($node->uc_auction['high_bid_amt']),
				$node->uc_auction['bid_count'],
				format_interval($node->uc_auction['expiry'] - $time),
				$node->uc_auction['high_bid_uid'] == $user->uid ? '<div class="uc-auction-high-bidder">' . t('Yes') . '</div>' : '<div class="uc-auction-not-high-bidder">' . t('No') . '</div>',
			);
		}
		if (!count($rows)) {
			$rows[] = array(
				array(
					'data' => t('There are no active auctions which @name has bid upon.', array('@name' => $user->name)),
					'colspan' => 5,
				),
			);
		}
	}
	else {
		// Show won or lost auctions. The query for this is fugly.
		$headers = array(t('Item'), t('High bid'), t('Bids'), t('Expired'));
		$rez = pager_query('SELECT DISTINCT(uab.nid) FROM {uc_auction_bids} uab INNER JOIN {uc_auction} ua ON ua.nid = uab.nid INNER JOIN {uc_auction_bids} uab2 ON ua.high_bid = uab2.bid AND uab2.uid ' . ($type === 'won' ? '=' : '<>') . 'uab.uid WHERE uab.uid = %d && ua.expiry < %d ORDER BY ua.expiry DESC', 20, 0, NULL, $user->uid, $time);

		while ($nid = db_fetch_object($rez)) {
			$node = node_load($nid->nid);
			$rows[] = array(
				l($node->title, "node/{$node->nid}"),
				uc_currency_format($node->uc_auction['high_bid_amt']),
				$node->uc_auction['bid_count'],
				t('@time ago', array('@time' => format_interval($time - $node->uc_auction['expiry'], variable_get('uc_auction_time_gran', 2)))),
			);
		}
		if (!count($rows)) {
			$rows[] = array(
				array(
					'data' => $type === 'won' ? t('There are no expired auctions which @name has won.', array('@name' => $user->name)) : t('There are no expired auctions which @name has lost.', array('@name' => $user->name)),
					'colspan' => 4,
				),
			);
		}
	}
	return theme('table', $headers, $rows, array('class' => 'uc-auction-user-history'));
}