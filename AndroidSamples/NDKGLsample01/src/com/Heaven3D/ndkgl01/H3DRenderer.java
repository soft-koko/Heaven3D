package com.Heaven3D.ndkgl01;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class H3DRenderer implements GLSurfaceView.Renderer 
{
	private static native void nativeInit();
	private static native void nativeResize(int w, int h);
	private static native void nativeRender();
	private static native void nativeDone();

	public void onSurfaceCreated(GL10 gl, EGLConfig config) 
	{
		nativeInit();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) 
	{
		nativeResize(w, h);
	}

	public void onDrawFrame(GL10 gl)
	{
		nativeRender();
	}
}