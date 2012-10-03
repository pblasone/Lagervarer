   $(document).ready(function(){
        $('#edit-sid').change(function() {
			if ($('#edit-sid option:selected').val() == '0') {
  				$('#edit-title').val('');
				$("[@name='notify_id']")[0].checked = true;
			}				   
			else {
				window.location = '?sid=' + $('#edit-sid option:selected').val();
			}
        });
    
    });