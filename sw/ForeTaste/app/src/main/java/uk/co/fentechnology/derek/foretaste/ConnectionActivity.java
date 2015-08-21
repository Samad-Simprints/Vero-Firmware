package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.ListView;

import java.util.ArrayList;

public class ConnectionAdapter extends BaseAdapter
{
  @Override
  public int getCount() {
    return 0;
  }

  @Override
  public Object getItem(int position)
  {
    return null;
  }

  @Override
  public long getItemId(int position)
  {
    return position;
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent)
  {
    return null;
  }
}

public class ConnectionActivity extends AppCompatActivity
{

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_connection);

    // Get the ArrayList from the Intent and give it to the adapter
    final ArrayList<String> contentList = getIntent().getStringArrayListExtra("connections");
    ArrayAdapter<String> contentAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,contentList);

    final ListView nameList = (ListView)findViewById(R.id.listView);
    nameList.setAdapter(contentAdapter);
    nameList.setOnItemClickListener(new AdapterView.OnItemClickListener()
    {
      @Override
      public void onItemClick(AdapterView<?> parent, View view, int position, long id)
      {
        Intent result = new Intent("derek.foretaste.RESULT_ACTION");
        result.putExtra("selection",position);
        setResult(Activity.RESULT_OK,result);
        Log.i("Clicked",contentList.get(position));
        finish();
      }
    });
  }

}
