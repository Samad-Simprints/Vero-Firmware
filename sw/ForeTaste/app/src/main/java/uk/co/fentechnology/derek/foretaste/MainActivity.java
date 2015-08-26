package uk.co.fentechnology.derek.foretaste;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.view.View;
import android.widget.EditText;

import com.simprints.scanner.library.Connector;

public class MainActivity extends AppCompatActivity {
  private Connector connector = Connector.getInstance();
  private String connectionString;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    connectionString = "No Connection";
    setContentView(R.layout.activity_main);
  }

  // request values
  static final int PICK_CONNECTION = 1;

  @Override
  protected void onActivityResult(int requestCode,int resultCode,Intent data) {
    // check which request this was
    if (requestCode==PICK_CONNECTION)
    {
      if (resultCode==RESULT_OK)
      {
        int selection = data.getIntExtra("selection",0);
        Log.i("result",String.valueOf(selection));
      }
    }
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    // Inflate the menu; this adds items to the action bar if it is present.
    getMenuInflater().inflate(R.menu.menu_my, menu);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    // Handle action bar item clicks here. The action bar will
    // automatically handle clicks on the Home/Up button, so long
    // as you specify a parent activity in AndroidManifest.xml.
    switch (item.getItemId())
    {
      case R.id.action_search:
        openSearch();
        return true;

      case R.id.action_settings:
        openSettings();
        return true;

      default:
        return super.onOptionsItemSelected(item);
    }
  }

  public void openSearch()
  {
    Log.i("openSearch", "called");
  }

  public void openSettings()
  {
    Intent intent = new Intent(this, ConnectionActivity.class);
    startActivityForResult(intent, PICK_CONNECTION);
  }
}
