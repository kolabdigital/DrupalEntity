/**
 * @file
 * jQuery Datatables
 */
(function ($) {

  Drupal.behaviors.kolab_lead_dt = {
    attach: function (context, settings) {

      jQuery.extend( jQuery.fn.dataTableExt.oSort, {
        "date-uk-pre": function ( a ) {
          var ukDatea = a.split('/');
          return (ukDatea[2] + ukDatea[1] + ukDatea[0]) * 1;
        },
        "date-uk-asc": function ( a, b ) {
          return ((a < b) ? -1 : ((a > b) ? 1 : 0));
        },
        "date-uk-desc": function ( a, b ) {
          return ((a < b) ? 1 : ((a > b) ? -1 : 0));
        }
      });


      //all leads
      if($('.lead-list').length) {
        var table = $('.lead-list').DataTable({
          //"searching": false,
          "lengthChange": false,
          "autoWidth": false,
          "fnDrawCallback": function(oSettings) {
            if (oSettings._iDisplayLength > oSettings.fnRecordsDisplay()) {
              $(oSettings.nTableWrapper).find('.dataTables_paginate').hide();
            }
          },
          "language": {
            "emptyTable": "No leads found"
          },
          "iDisplayLength" : 20,
          "order": [[ 0, "desc" ]],
          "columnDefs": [ {
            "targets": [ 6 ],
            "sortable": false
          }, {
            "targets": [ 8 ],
            "sortable": false
          }]
        });

        table.draw();

        $('#leads-filter input').keyup(function(){
          table.search($(this).val()).draw();
        })
      }
      
      //recent leads
      if($('.lead-recent-list').length) {
        var table_recent = $('.lead-recent-list').DataTable({
          "searching": false,
          "lengthChange": false,
          "paging": false,
          "autoWidth": false,
          "language": {
            "emptyTable": "No leads found"
          },
          "fnDrawCallback": function(oSettings) {
            if (oSettings._iDisplayLength > oSettings.fnRecordsDisplay()) {
              $(oSettings.nTableWrapper).find('.dataTables_paginate').hide();
            }
          },
          "order": [[ 3, "desc" ]],
          "columnDefs": [{
            "sType": "date-uk",
            "targets": [ 3 ]
          }]
        });

        table_recent.draw();
      }


    }
  };
})(jQuery);