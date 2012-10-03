
/*
If we use this code on pages where ajax is used to manipulate the dom, we should use this wrapper instead:
Drupal.behaviors.myModuleBehavior = function (context) {
See http://drupal.org/node/304258
*/

   $(document).ready(function(){
   
   		if ($("form#uc-custom-product-admin-form")) {

			$("form#uc-custom-product-admin-form").submit(function() {
				if ($("select#edit-bulk-action").val() == 'delete') {
					if (!confirm('De afkrydsede varer/partier vil blive slettet. Ønsker du at fortsætte?')) return false;
				}	
			
			});
	
			$("select#edit-bulk-action").change(function() {
				if ($(this).val() == 'category') {
					$("select#edit-category").slideDown();	
				} else if ($("select#edit-category").is(":visible")) {
					$("select#edit-category").slideUp();
				}
			
			});
			
			$("input#toggle_chk_").click(function() {
				var bChecked = $(this).is(':checked');
				$("input[type=checkbox]").each(function() {
					if ($(this).attr('name').search('chk_') === 0) {
						$(this).attr('checked', bChecked);
					}
				});
			});
			
		}
		
		if ($("form#node-form")) {
		
			if ($("input[@name='field_price_per_piece[value]']:checked").val() == '0') {
				$("#edit-default-qty-wrapper").hide();                
	            $("#edit-default-qty").val('1');
			} else {
				$("#edit-pkg-qty-wrapper").hide();
	            resetMinQty();
			}
	    
		    $("input[@name='field_price_per_piece[value]']").click(function() {
				if ($("input[@name='field_price_per_piece[value]']:checked").val() == '0') {
					$("#edit-default-qty-wrapper").slideUp();                
					$("#edit-default-qty").val('1');
	                $("#edit-pkg-qty-wrapper").slideDown();                             
	                
					$("#edit-is-multiple").removeAttr("disabled");                                
				} else {
					$("#edit-pkg-qty-wrapper").slideUp();      
	                resetMinQty();
					$("#edit-default-qty-wrapper").slideDown();                
	
					$("#edit-is-multiple").attr("checked","checked");               
					$("#edit-is-multiple").attr("disabled","disabled");                
				}
			});                         
	        
	        $('#edit-copy-this').click(function() {   
				if ($('#edit-copy-this:checked').val() == 1 && $('#edit-consumer-description').val() == '') {
					$('#edit-consumer-description').val($('#edit-body').val());					
				}
	        });
			
		}
	    
	});
	    
	    function resetMinQty() {
			if ($("#edit-default-qty").val() < 5) {
		        $("#edit-default-qty").val('5');
	        }
	    }
    
/*    $("input[@name='field_price_per_piece[value]']").change(function() {
		if ($("input[@name='field_price_per_piece[value]']:checked").val())
			$(".group").toggleClass("group_vert");
		else
			$(".group").toggleClass("group_vert");
			$(this).blur();
		}
	);*/