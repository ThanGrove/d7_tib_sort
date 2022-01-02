(function ($) {


  Drupal.behaviors.tibarmy_sort = {
    attach: function (context, settings) {
      // Resubmit form upon sort order change
      $('select#edit-sort-order').change(function(e) {
        var selectEl = $(e.target);
        var srchform = selectEl.parents('#views-exposed-form-search-page').eq(0);
        srchform.submit();
      });
    }
  }

  // Function to ensure that string entered has a shad.
  function ensureShay(srchstr) {
    srchstr = srchstr.trim();
    var lastchar = srchstr.substr(srchstr.length - 1, 1);
    var lastcharcode = lastchar.charCodeAt(0);
    // Check if Tibetan
    if (lastcharcode > 3839 && lastcharcode < 4096) {
      // Check if shad or tsek
      if (lastchar === '་') {
        srchstr = srchstr.substr(0, srchstr.length - 1) + '།';
      } else if (lastchar !== '།') {
        // If not add shad
        srchstr += '།';
      }
    }
    return srchstr;
  }

  Drupal.behaviors.tibarmy_test = {
    attach: function (context, settings) {
      // Ensure that search string ends in a shad
      if (context == document) {
        // On home page validate input on submit
        $('#views-exposed-form-solr-search-page').submit(function (e) {
          // trim whitespace
          var srchstr = $('#edit-keyword-solr').val();
          // Call function to ensure shad
          srchstr = ensureShay(srchstr);  // Function above
          if (srchstr.charAt(0) !== '"' && srchstr.indexOf(' ') > -1) {
            srchstr = srchstr.replace(/ /g, '+');
          }
          // Replace with new value
          $('#edit-keyword-solr').val(srchstr);
        });

        // On search results page, need to use key up to validate input
        $('body.page-solr-search #edit-keyword-solr').on('keyup', function (e) {
          // When enter is pressed ...
          if (e.which === 13) {
            // ensure shad
            var srchstr = $(e.target).val();
            srchstr = ensureShay(srchstr);
            if (srchstr.charAt(0) !== '"' && srchstr.indexOf(' ') > -1) {
              srchstr = srchstr.replace(/ /g, '+');
            }
            $('#edit-keyword-solr').val(srchstr);
          }
        });

        // Enable the remove search string minus icon on search page
        if ($('body.page-solr-search .current-search-item-active-items li.first a').length > 0) {
          var firsthref = $('body.page-solr-search .current-search-item-active-items li.first a').attr('href');
          var hrefpts = firsthref.split('?keyword_solr=');
          if (hrefpts?.length > 1) {
            var sstr = hrefpts[1];
            var sstrpts = sstr.split('&');
            sstrpts.shift();
            var newhref = hrefpts[0];
            if (sstrpts.length > 0) {
              newhref += '?' + sstrpts.join('&');
            }
            $('body.page-solr-search .current-search-item-active-items li.first a').attr('href', newhref);
          }
        }
      }
    }
  }
}(jQuery));
