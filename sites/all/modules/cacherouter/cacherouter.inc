<?php
/**
 * @file cacherouter.inc
 *   Defines the cacherouter module
 */
require dirname(__FILE__) .'/CacheRouter.php';

// We set this to true, and let the engines determine if they can implement it.  (e.g. db can't.)
$conf['page_cache_fastpath'] = TRUE;

function cache_get($key, $bin = 'cache') {
  global $cache, $user;
  if (!isset($cache)) {
    $cache = new CacheRouter();
  }

  $cache_object = $cache->get($key, $bin);  
  if ($cache_object && isset($user->cache) && $user->cache > $cache_object->created) {
    return 0;
  }
  
  return $cache_object;
}

function cache_set($key, $bin = 'cache', $value, $expire = CACHE_PERMANENT, $headers = NULL) {
  global $cache;
  if (!isset($cache)) {
    $cache = new CacheRouter();
  }
  return $cache->set($key, $value, $expire, $headers, $bin);
}

function cache_clear_all($key = NULL, $bin = NULL, $wildcard = FALSE) {
  global $cache;
  global $user;
  
  if (!isset($cache)) {
    $cache = new CacheRouter();
  }
  
  if (!isset($key) && !isset($bin)) {
    // Clear the block cache first, so stale data will
    // not end up in the page cache.
    $cache->flush('cache_block');
    $cache->flush('cache_page');
    return;
  }
  
  if (empty($key)) {
    if (variable_get('cache_lifetime', 0)) {
      // We store the time in the current user's $user->cache variable which
      // will be saved into the sessions table by sess_write(). We then
      // simulate that the cache was flushed for this user by not returning
      // cached data that was cached before the timestamp.
      $user->cache = time();

      $cache_flush = variable_get('cache_flush', 0);
      if ($cache_flush == 0) {
        // This is the first request to clear the cache, start a timer.
        variable_set('cache_flush', time());
      }
      else if (time() > ($cache_flush + variable_get('cache_lifetime', 0))) {
        // Clear the cache for everyone, cache_flush_delay seconds have
        // passed since the first request to clear the cache.
        $cache->flush($bin);
        variable_set('cache_flush', 0);
      }
    }
    else {
      // No minimum cache lifetime, flush all temporary cache entries now.
      $cache->flush($bin);
    }
  }
  else {
    if ($wildcard) {
      if ($cid == '*') {
        $cache->flush($bin);
      }
      else {
        $cache->delete($key . '*', $bin);
      }
    }
    else {
      $cache->delete($key, $bin);
    }
  }

}

/**
 * Main callback from DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE phase
 */
function page_cache_fastpath() {
  global $base_root;
  global $cache;
  
  if (empty($_POST) && !isset($_COOKIE['cacherouter'])) {
    if (!isset($cache)) {
      $cache = new CacheRouter();
    }
    if ($cache->page_fast_cache('cache_page')) {
      $page = $cache->get($base_root . request_uri(), 'cache_page');
      if (!empty($page)) {
        drupal_page_header();
        
         //checking if first chars are compressed (always the same pattern for every header)
        if (substr($page->data, 0,3) == "\x1f\x8b\x08") {
          
          // Determine if the browser accepts gzipped data.
          if (@strpos($_SERVER['HTTP_ACCEPT_ENCODING'], 'gzip') === FALSE && function_exists('gzencode')) {
            // Strip the gzip header and run uncompress.
            $page->data = gzinflate(substr(substr($page->data, 10), 0, -8));
          }
          elseif (function_exists('gzencode')) {
            //send gzip header to the browser
            header('Content-Encoding: gzip');
          }
        }
        
        print $page->data;
        return TRUE;
      }
    }
  }
  return FALSE;
}
