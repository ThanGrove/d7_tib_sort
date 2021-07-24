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
}(jQuery));
