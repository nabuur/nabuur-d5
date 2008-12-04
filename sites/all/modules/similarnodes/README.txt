This module provides a way to find similar nodes by taxonomy using views.

<h2>Features:</h2>
 <ul>
 <li>Weighting by vocabulary - Say you have two vocabularies on a movie site "Genre" and "Community Tags", the latter being a free tagging taxonomy.  With this module, you can give Genre a weight of 10 and Community tags a weight of 1.  Which means that if I'm looking at "Meet the Parents", I'm most likely to see a list of similar Romantic Comedies like "Sleepless in Seattle" and "French Kiss" with something like "Taxi" or "The Godfather II" coming lower in the list, because someone tagged both "Robert De niro"</li>

<li> Sorting - Sorts by weight... pretty simple </li>

<li> Filtering - I want to see only those with a weight higher than 15... this is a little arbitrary, and you are better off using the pager / limiting results, but it does work </li>
</ul>

<h3> Usage: </h3>
<ol>
 <li> Install the module (duh)</li>
 <li> Go to admin/settings/similarnodes</li>
 <li> Set the weight for each taxonomy </li>
 <li> Now create a view, add the argument "SimilarNodes: Source Node Id" - You need this to make it work</li>
 <li> Add the field SimilarNodes: Weight </li>
 <li> give it a shot! go to yourview/{NID} for a list of related nodes </li>
</ol>