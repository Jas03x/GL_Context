//
//  Framebuffer.cpp
//  RoughRocket
//
//  Created by Jas S on 2017-05-04.
//  Copyright © 2017 Jas S. All rights reserved.
//

#include "Framebuffer.h"

void FBO::init(unsigned int width, unsigned int height)
{
	this->dimensions = glm::uvec2(width, height);
    glGenFramebuffers(1, &this->id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void FBO::check(unsigned int attachments)
{
	GLuint status = 0;

	if (attachments > 0)
	{
		GLenum* DRAW_BUFFERS = new GLenum[attachments];
		for (unsigned int i = 0; i < attachments; i++)
			DRAW_BUFFERS[i] = GL_COLOR_ATTACHMENT0 + i;
		glDrawBuffers(attachments, DRAW_BUFFERS);

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		delete[] DRAW_BUFFERS;
	} 
	else
	{
		glDrawBuffer(GL_NONE);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("OpenGL error %i while trying to create framebuffer: ", glGetError());
		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			puts("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			puts("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			puts("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			puts("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			puts("GL_FRAMEBUFFER_UNSUPPORTED");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			puts("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;
		default:
			puts("GL_FRAMEBUFFER_UNDEFINED");
			break;
		}
		throw -1;
	}
}

void FBO::destroy()
{
    if(glIsFramebuffer(this->id) == GL_TRUE) glDeleteFramebuffers(1, &this->id);
}

/*
* Framebuffer: 
*/

void Framebuffer::initalize(unsigned int width, unsigned int height, Attachment::Type texture_type, Attachment::Type depth_type)
{
	FBO::init(width, height);

	if (depth_type == Attachment::NONE) {
		glGenRenderbuffers(1, &this->depth_attachment);
		glBindRenderbuffer(GL_RENDERBUFFER, this->depth_attachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_attachment);
	} else {
		this->depth_attachment = Attachment::get(depth_type, width, height);
	}

	if (texture_type != Attachment::NONE)
	{
		this->texture_attachment = Attachment::get(texture_type, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture_attachment, 0);
		this->check(1);
	}
	else
		this->check(0);
}

void Framebuffer::destroy()
{
	if (glIsTexture(this->texture_attachment) == GL_TRUE) glDeleteTextures(1, &this->texture_attachment);
	if (glIsTexture(this->depth_attachment) == GL_TRUE) glDeleteTextures(1, &this->depth_attachment);
	if (glIsRenderbuffer(this->depth_attachment) == GL_TRUE) glDeleteTextures(1, &this->depth_attachment);
	FBO::destroy();
}
