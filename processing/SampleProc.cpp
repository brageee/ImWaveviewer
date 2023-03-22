/*
 *  Copyright (C) 2015, Mike Walters <mike@flomp.net>
 *
 *  This file is part of inspectrum.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>
#include "SampleProc.h"

template <typename Tin, typename Tout>
SampleProc<Tin, Tout>::SampleProc(std::shared_ptr<SampleSource<Tin>> src) : src(src)
{
	
}

template <typename Tin, typename Tout>
std::unique_ptr<std::vector<Tout>> SampleProc<Tin, Tout>::GetSamples(size_t start, size_t length)
{
    // TODO: base this on the actual history required  
    size_t localStart, localHistory;
    localHistory = history;
    if(start < history)
    {
        localStart = start;
        localHistory = 0;
    } else
        localStart = start-history;
    
    if(src->Count() < length+history)
        localHistory = length;

    auto samples = src->GetSamples(localStart, length + localHistory);
    if (samples == nullptr)
    	return nullptr;

    auto temp = std::make_unique<std::vector<Tout>>(localHistory + length);
    auto dest = std::make_unique<std::vector<Tout>>(length);
    //QMutexLocker ml(&mutex);
    Work(samples.get(), temp.get(), localHistory + length, start);            
    for (size_t i = localHistory; i < localHistory+length; i++)
    {
        dest.get()->at(i-localHistory) = temp.get()->at(i);
    }        
    return dest;
}

/*template <typename Tin, typename Tout>
void SampleProc<Tin, Tout>::invalidateEvent()
{
	SampleSource<Tout>::invalidate();
}*/

template class SampleProc<std::complex<float>, std::complex<float>>;
template class SampleProc<std::complex<float>, float>;
template class SampleProc<float, float>;
