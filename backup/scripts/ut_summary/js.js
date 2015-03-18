function getPercent()
{
	sitem = document.getElementsByClassName("success"); 
        eitem = document.getElementsByClassName("error");
	sl = sitem.length + eitem.length; 
	if(sl == 0) return -1;
	return sitem.length/sl;
}

function updateBar()
{
	items_ok = document.getElementById("items_ok");
	items_err = document.getElementById("items_err");

	percent = getPercent();
	if(percent == -1)
	{
	 items_ok.setAttribute("width", "50%" );
	 items_ok.textContent = "??";
	 items_err.setAttribute("width", "50%" );
	 items_err.textContent = "??";
	}
	else
	{
	 items_ok.setAttribute("width", (percent*100)+"%" );

	if(percent == 0)
 	  items_ok.textContent = "";
	else
	 items_ok.textContent = (percent*100)+"%";



	 items_err.setAttribute("width", (100-percent*100)+"%" );

	 if(percent == 1)
 	  items_err.textContent = "";
	 else
	  items_err.textContent = (100-percent*100)+"%";
	}
}

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
			if(filters[j] == "" || filters[j] == " ") continue;

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

       while( sitem.length > 0) 
	 sitem[0].className = "unvisible_success";
      
      	while( eitem.length > 0) 
	 eitem[0].className = "unvisible_error";
      
	vals = val.split(" ");
      	
	sitem = document.getElementsByClassName("unvisible_success")
	eitem = document.getElementsByClassName("unvisible_error")

	if(vals.indexOf("[err]") != -1 )
		sitem = [];

        if(vals.indexOf("[ok]") != -1 )
		eitem = [];

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
	 if(items[i].className == "unvisible_success")
		items[i].className = "success";
	if(items[i].className == "unvisible_error")
		items[i].className = "error";
	}
     
	updateBar();
    }


