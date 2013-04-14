package com.Heaven3D.ndkgl01;

import android.opengl.GLSurfaceView;
import android.content.Context;


class H3DSurfaceView extends GLSurfaceView 
{
	H3DRenderer render_;

	public H3DSurfaceView(Context context)
	{
		super(context);
		render_ = new H3DRenderer();
		setRenderer(render_);
    }
}