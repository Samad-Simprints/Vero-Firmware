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

public class ConnectionActivity extends AppCompatActivity
{
  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_connection);

    ConnectionAdapter connectionAdapter = new ConnectionAdapter(this);

    final ListView nameList = (ListView)findViewById(R.id.listView);
    nameList.setAdapter(connectionAdapter);
    nameList.setOnItemClickListener(new AdapterView.OnItemClickListener()
    {
      @Override
      public void onItemClick(AdapterView<?> parent, View view, int position, long id)
      {
        Intent result = new Intent("derek.foretaste.RESULT_ACTION");
        result.putExtra("selection",position);
        setResult(Activity.RESULT_OK,result);
        Log.i("Clicked","position " + position);
        finish();
      }
    });
  }
}
