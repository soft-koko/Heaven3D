package com.Heaven3D.ndkgl01;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity
{
	private H3DSurfaceView surface_view_;
	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		surface_view_ = new H3DSurfaceView(this);
		setContentView(surface_view_);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}

	static 
	{
		System.loadLibrary("ndkgl01");
	}
}