package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

public class SelectionActivity extends AppCompatActivity
{

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_selection);

    SelectionAdapter selectionAdapter = new SelectionAdapter(this,getIntent().getIntExtra("index",0));

    final ListView nameList = (ListView)findViewById(R.id.listScannerView);
    nameList.setAdapter(selectionAdapter);
    nameList.setOnItemClickListener(new AdapterView.OnItemClickListener()
    {
      @Override
      public void onItemClick(AdapterView<?> parent, View view, int position, long id)
      {
        Intent result = new Intent("derek.foretaste.RESULT_ACTION");
        result.putExtra("scanner", position);
        setResult(Activity.RESULT_OK, result);
        Log.i("Clicked", "position " + position);
        finish();
      }
    });
  }

}
