function getItemTestClass(item)
{
	el = item.getElementsByClassName("test_class")
	return el[0].textContent;
}

function getItemTestName(item)
{
	el = item.getElementsByClassName("test_name")
	return el[0].textContent;
}

function getItemTestResult(item)
{
	el = item.getElementsByClassName("test_result")
	return el[0].textContent;
}

function applyFilter(items, filters)
{
	ret_item = [];
	for( i=0; i<items.length; i++)
 	{
		test_class = getItemTestClass(items[i]).toLowerCase();
		test_name = getItemTestName(items[i]).toLowerCase();
		test_result = getItemTestResult(items[i]).toLowerCase();

		for(j=0; j<filters.length;j++)
		{
			if(filters[j] == "") continue;

			if(test_class.indexOf(filters[j].toLowerCase()) != -1 
		 		|| test_name.indexOf(filters[j].toLowerCase()) != -1 
				|| test_result.indexOf(filters[j].toLowerCase()) != -1)
			{
				ret_item.push(items[i]);
				break;
			}
		}
	}

	return ret_item;
}

function onFilter(val) 
    {

      sitem = document.getElementsByClassName("success");
      eitem = document.getElementsByClassName("error");
      citems = [];

      for( i=0; i<sitem.length; i++) 
	{ 
	 sitem[i].style.visibility = "hidden"; 
	 sitem[i].style.height = 0; 
	}
      
      	for( i=0; i<eitem.length; i++) 
      	{
         eitem[i].style.visibility = "hidden"; 
	 eitem[i].style.height = 0; 
      	}
      
	vals = val.split(" ");
      	
	if(vals.indexOf("[err]") != -1 )
        {
		sitem = [];
	}
        
        if(vals.indexOf("[ok]") != -1 )
        {
		eitem = [];
	}
	
	for( i=0; i<sitem.length; i++) 
	{ 
	  citems.push(sitem[i]);
	}
      
      	for( i=0; i<eitem.length; i++) 
      	{
      	   citems.push(eitem[i]);
      	}
	
	if(vals.length == 0 || (vals.length == 1 && (vals[0] == "[err]" || vals[0] == "[ok]" || vals[0] == "")))
		items = citems;
	else
		items = applyFilter(citems, vals);
      
	for( i=0; i<items.length; i++) 
	{ 
	 items[i].style.visibility = "visible"; 
	 items[i].style.height = 35; 
	}
     
    }
