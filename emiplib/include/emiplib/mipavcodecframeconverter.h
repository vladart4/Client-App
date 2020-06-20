/*
    
  This file is a part of EMIPLIB, the EDM Media over IP Library.
  
  Copyright (C) 2006-2016  Hasselt University - Expertise Centre for
                      Digital Media (EDM) (http://www.edm.uhasselt.be)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA

*/

/**
 * \file mipavcodecframeconverter.h
 */

#ifndef MIPAVCODECFRAMECONVERTER_H

#define MIPAVCODECFRAMECONVERTER_H

#include "mipconfig.h"

#ifdef MIPCONFIG_SUPPORT_AVCODEC

#include "mipcomponent.h"
#include "miptime.h"
#include <unordered_map>

extern "C" 
{	
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
}

#include <list>

class MIPVideoMessage;

/** Convert video frames to a desired width, height and format using libavcodec.
 *  This component will convert incoming video frames to a specific width, height
 *  and format. To do this, the libavcodec library is used.
 */
class EMIPLIB_IMPORTEXPORT MIPAVCodecFrameConverter : public MIPComponent
{
public:
	MIPAVCodecFrameConverter();
	~MIPAVCodecFrameConverter();

	/** Initializes the component and instructs it to convert messages to a specific width,
	 *  height and raw video frame subtype. 
	 *  Initializes the component and instructs it to convert messages to a specific width,
	 *  height and raw video frame subtype. Set \c targetWidth and \c targetHeight to -1 to
	 *  conserve the original width and height of the frame.
	 */
	bool init(int targetWidth, int targetHeight, uint32_t targetSubtype);

	/** Cleans up the component. */
	bool destroy();

	bool push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg);
	bool pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg);

	/** Initializes the libavcodec library.
	 *  This function initializes the libavcodec library. The library should only be initialized once
	 *  in an application.
	 */
	static void initAVCodec();
private:
	void clearMessages();
	void expire();
	void clearCache();

	bool m_init;
	std::list<MIPVideoMessage *> m_messages;
	std::list<MIPVideoMessage *>::const_iterator m_msgIt;
	int64_t m_lastIteration;

	int m_targetWidth;
	int m_targetHeight;
	uint32_t m_targetSubtype;
	AVPixelFormat m_targetPixFmt;

	class ConvertCache 
	{
	public:
		ConvertCache(int srcWidth, int srcHeight, uint32_t srcSubtype, SwsContext *pSwsContext)
		{
			m_srcWidth = srcWidth;
			m_srcHeight = srcHeight;
			m_srcSubtype = srcSubtype;
			m_pSwsContext = pSwsContext;
			m_lastTime = MIPTime::getCurrentTime();
		}

		~ConvertCache()
		{
			sws_freeContext(m_pSwsContext);
		}

		int getSourceWidth() const						{ return m_srcWidth; }
		int getSourceHeight() const						{ return m_srcHeight; }
		uint32_t getSourceSubtype() const					{ return m_srcSubtype; }
		SwsContext *getSwsContext()						{ return m_pSwsContext; }

		MIPTime getLastUpdateTime() const					{ return m_lastTime; }
		void setLastUpdateTime(MIPTime t)					{ m_lastTime = t; }
	private:
		SwsContext *m_pSwsContext;
		int m_srcWidth;
		int m_srcHeight;
		uint32_t m_srcSubtype;
		MIPTime m_lastTime;
	};
	std::unordered_map<uint64_t, ConvertCache *> m_convertCache;
	MIPTime m_lastExpireTime;
};

#endif // MIPCONFIG_SUPPORT_AVCODEC

#endif // MIPAVCODECFRAMECONVERTER_H

