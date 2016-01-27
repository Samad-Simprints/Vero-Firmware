package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;
import com.simprints.scanner.library.Scanner;
import com.simprints.scanner.library.ScannerCallback;
import com.simprints.scanner.library.Template;
import com.simprints.scanner.library.Image;

public class MainActivity extends AppCompatActivity implements ScannerCallback {
  private Connector connector = Connector.getInstance();
  private static Connection connection = null;
  private static int connectionIndex;
  private static Scanner scanner;
  private ViewHolder viewHolder;
  private boolean boPowerOn = false;
  private boolean boButtonEnabled = false;
  private boolean boScanReady = false;
  private boolean boTemplateReady = false;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    viewHolder = new ViewHolder(this);
    viewHolder.update();

    Log.i("MainActivity", "onCreate");
  }

  @Override
  public void onStatusChange(String message)
  {
    // @TODO: something useful
    Log.v("onStatusChange", message);
    switch(message)
    {
      case "image:scan complete":
        boScanReady = true;
        break;

      case "image:transfer complete":
        boScanReady = false;
        break;

      case "template:conversion complete":
        boTemplateReady = true;
        break;

      case "template:transfer complete":
        boTemplateReady = false;
        break;
    }

    viewHolder.update();
  }

  // request values
  static final int PICK_CONNECTION = 1;

  @Override
  protected void onActivityResult(int requestCode,int resultCode,Intent data) {
    // check which request this was
    if (resultCode==RESULT_OK)
    {
      switch (requestCode)
      {
        case PICK_CONNECTION:
          int selection = data.getIntExtra("selection",0);
          Connection c = connector.get(selection);
          if (c.isSetup())
          {
            connection = c;
            connectionIndex = selection;
          }
          else
          {
            Toast.makeText(this,c.name() + " is not ready",Toast.LENGTH_SHORT).show();
          }
          break;
      }
      viewHolder.update();
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

  public void openSearch() {
    Log.i("openSearch", "called");
  }

  public void openSettings() {
    Intent intent = new Intent(this, ConnectionActivity.class);
    startActivityForResult(intent, PICK_CONNECTION);
  }

  // button handler - auto select connection
  public void selectAutoConnection(View view)
  {
    if (connection == null)
    {
      connection = connector.autoConnect();
    }
    viewHolder.update();
  }

  // button handler - select/clear connection
  public void selectConnection(View view)
  {
    if (connection == null)
    {
      openSettings();
    }
    else
    {
      connection = null;
      scanner = null;
    }
    viewHolder.update();
  }

  // button handler - connect/clear scanner
  public void connectScanner(View view) {
    if (scanner == null)
    {
      scanner = new Scanner(connection,this);
    }
    else
    {
      scanner = null;
    }
    viewHolder.update();
  }

  // button handler - scanTemplateBtn
  public void scanTemplate(View view) {
    int err = scanner.getTemplate();
  }

  // button handler - getTemplateBtn
  public void getTemplate(View view) {
    Template template = scanner.readTemplate();
    viewHolder.showTemplate(template.getData());
  }

  // button handler - scanImageBtn
  public void scanImage(View view) {
    int err = scanner.getImage();
  }

  // button handler - getImageBtn
  public void getImage(View view) {
    Image image = scanner.readImage();
    viewHolder.showImage(image.getData());
  }

  // button handler - power
  public void un20Power(View view) {
    if (Scanner.ERROR_NONE == scanner.setUn20bPower(!boPowerOn))
    {
      boPowerOn = !boPowerOn;
      viewHolder.update();
    }
  }

  // button handler - button
  public void un20Button(View view) {
    if (Scanner.ERROR_NONE == scanner.setButton(!boButtonEnabled))
    {
      boButtonEnabled = !boButtonEnabled;
      viewHolder.update();
    }
  }

  // button handler - battery
  public void battery(View view) {
    viewHolder.showBattery(scanner.getBatteryPercent());
  }

  // button handler - quality
  public void quality(View view) {
    viewHolder.showQuality(scanner.getImageQuality());
  }

  // local class to handle view and updates
  class ViewHolder {
    // connection
    private TextView connectNameTxt;
    private Button selectScannerBtn;
    private TextView scannerNameTxt;
    private Button selectAutoConnectionBtn;
    private Button selectConnectionBtn;
    // actions
    private Button scanTemplateBtn;
    private Button getTemplateBtn;
    private Button scanImageBtn;
    private Button getImageBtn;
    private Button batteryBtn;
    private Button qualityBtn;
    private Button powerBtn;
    private Button buttonBtn;
    //report
    private TextView templateTxt;

    public ViewHolder(Activity activity)
    {
      connectNameTxt = (TextView) activity.findViewById(R.id.connectName);
      selectAutoConnectionBtn = (Button) activity.findViewById(R.id.connectAutoBtn);
      selectConnectionBtn = (Button) activity.findViewById(R.id.connectBtn);
      scannerNameTxt = (TextView) activity.findViewById(R.id.scannerName);
      selectScannerBtn = (Button) activity.findViewById(R.id.scannerBtn);
      scanTemplateBtn = (Button) activity.findViewById(R.id.scanTemplateBtn);
      getTemplateBtn = (Button) activity.findViewById(R.id.getTemplateBtn);
      scanImageBtn = (Button) activity.findViewById(R.id.scanImageBtn);
      getImageBtn = (Button) activity.findViewById(R.id.getImageBtn);
      batteryBtn = (Button) activity.findViewById(R.id.batteryBtn);
      qualityBtn = (Button) activity.findViewById(R.id.qualityBtn);
      powerBtn = (Button) activity.findViewById(R.id.powerBtn);
      buttonBtn = (Button) activity.findViewById(R.id.buttonBtn);
      templateTxt = (TextView) activity.findViewById(R.id.templateTxt);
    }

    public void setConnectName() {
      connectNameTxt.setText((connection == null) ? "No Connection" : connection.name());
    }

    public void setScannerName() {
      scannerNameTxt.setText((scanner == null) ? "None": connection.deviceName());
    }

    public void update()
    {
      if (connection==null)
      {
        selectConnectionBtn.setText("Select");
        selectAutoConnectionBtn.setEnabled(true);
        selectScannerBtn.setEnabled(false);
      }
      else
      {
        selectConnectionBtn.setText("Clear");
        selectAutoConnectionBtn.setEnabled(false);
        selectScannerBtn.setEnabled(true);
      }

      if (scanner==null)
      {
        selectScannerBtn.setText("Connect");
        scanTemplateBtn.setEnabled(false);
        scanImageBtn.setEnabled(false);
        batteryBtn.setEnabled(false);
        qualityBtn.setEnabled(false);
        powerBtn.setEnabled(false);
        buttonBtn.setEnabled(false);
        boButtonEnabled = false;
        boPowerOn = false;
      }
      else
      {
        selectScannerBtn.setText("Clear");
        scanTemplateBtn.setEnabled(true);
        scanImageBtn.setEnabled(true);
        batteryBtn.setEnabled(true);
        qualityBtn.setEnabled(true);
        powerBtn.setEnabled(true);
        buttonBtn.setEnabled(true);
      }

      setConnectName();
      setScannerName();

      getImageBtn.setEnabled(boScanReady);
      getTemplateBtn.setEnabled(boTemplateReady);

      powerBtn.setText(boPowerOn? "Power\nOff": "Power\nOn");
      buttonBtn.setText(boButtonEnabled? "Button\nOff": "Button\nOn");
    }

    public void showTemplate(byte[] r) {
      String str = "Template: ";

      for ( byte b : r )
      {
        str = str.concat(String.valueOf((char)b));
      }

      templateTxt.setText(str);
    }

    public void showImage(byte[] r) {
      String str = "Image: ";

      for ( byte b : r )
      {
        str = str.concat(String.valueOf((char)b));
      }

      templateTxt.setText(str);
    }

    public void showBattery(int b) {
      String str = "Battery: " + b +"%";
      templateTxt.setText(str);
    }

    public void showQuality(int b) {
      String str = "Quality: " + b +"%";
      templateTxt.setText(str);
    }
  }

}
