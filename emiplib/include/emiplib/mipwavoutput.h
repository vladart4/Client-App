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
 * \file mipwavoutput.h
 */

#ifndef MIPWAVOUTPUT_H

#define MIPWAVOUTPUT_H

#include "mipconfig.h"
#include "mipcomponent.h"

class MIPWAVWriter;

/** A sound file output component.
 *  This component writes incoming sound data to a file. It uses an internal simple
 *  WAV file write to accomplish this. Incoming messages have to be unsigned 8 bit audio messages.
 *  No messages are generated by this component.
 */
class EMIPLIB_IMPORTEXPORT MIPWAVOutput : public MIPComponent
{
public:
	MIPWAVOutput();
	~MIPWAVOutput();

	/** Opens a sound file.
	 *  This function opens a sound file.
	 *  \param fname The name of the file.
	 *  \param sampRate The sampling rate.
	 */
	bool open(const std::string &fname, int sampRate);

	/** Closes the sound file.
	 *  Using this function, a previously opened sound file can be closed again.
	 */
	bool close();

	bool push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg);
	bool pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg);
private:
	MIPWAVWriter *m_pSndFile;
	int m_sampRate;
};

#endif // MIPWAVOUTPUT_H

