/*
 * Copyright (c) Xerox Corporation 1997. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Linking this file statically or dynamically with other modules is making
 * a combined work based on this file.  Thus, the terms and conditions of
 * the GNU General Public License cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of this file
 * give you permission to combine this file with free software programs or
 * libraries that are released under the GNU LGPL and with code included in
 * the standard release of ns-2 under the Apache 2.0 license or under
 * otherwise-compatible licenses with advertising requirements (or modified
 * versions of such code, with unchanged license).  You may copy and
 * distribute such a system following the terms of the GNU GPL for this
 * file and the licenses of the other code concerned, provided that you
 * include the source code of that other code when and as the GNU GPL
 * requires distribution of source code.
 *
 * Note that people who make modified versions of this file are not
 * obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so.  The GNU General Public License
 * gives permission to release a modified version without this exception;
 * this exception also makes it possible to release a modified version
 * which carries forward this exception.
 */
[[maybe_unused]]
static const char rcsid[] =
	"@(#) $Header: /cvsroot/nsnam/ns-2/adc/timewindow-est.cc,v 1.7 2005/08/26 05:05:28 tomh Exp $";


//Time Window estimation

#include "estimator.h"
#include <stdlib.h>

class TimeWindow_Est : public Estimator {
public:
	TimeWindow_Est() :scnt(1),maxp(0){bind("T_",&T_);};
	inline void change_avload(double incr) { avload_ += incr; if (incr >0) scnt=0;}
protected:
	void estimate();
	int scnt;
	double maxp;//maximum of previous interval
	int T_;
};

void TimeWindow_Est::estimate() {
	measload_ = meas_mod_->bitcnt()/period_;
	if (meas_mod_->bitcnt()/period_ >avload_)
		avload_=meas_mod_->bitcnt()/period_;
	if (maxp < meas_mod_->bitcnt()/period_)
		maxp=meas_mod_->bitcnt()/period_;

	if (scnt == T_)  
		{
			scnt-=T_;
			avload_=maxp;
			maxp=0;
		}
	//printf("%f %f %f\n",Scheduler::instance().clock(),avload_,meas_mod_->bitcnt()/period_);
	
	fflush(stdout);
	meas_mod_->resetbitcnt();
	scnt++;
}

static class TimeWindow_EstClass : public TclClass {
public:
	TimeWindow_EstClass() : TclClass ("Est/TimeWindow") {}
	TclObject* create(int,const char*const*) {
		return (new TimeWindow_Est());
	}
}class_timewindow_est;

