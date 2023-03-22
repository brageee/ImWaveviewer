#include "AbstractSampleSource.h"

AbstractSampleSource::AbstractSampleSource()
{
	id = processing::utils::GetRandomUInt();
}

void AbstractSampleSource::subscribe(std::shared_ptr<AbstractSampleSource> subscriber)
{
	subscribers.insert(subscriber);
}

/*void AbstractSampleSource::invalidate()
{
	for (auto subscriber : subscribers) {
		subscriber->invalidateEvent();
	}
}*/

int AbstractSampleSource::subscriberCount()
{
	return subscribers.size();
}

void AbstractSampleSource::unsubscribe(std::shared_ptr<AbstractSampleSource> subscriber)
{
	subscribers.erase(subscriber);
}

 std::set<std::shared_ptr<AbstractSampleSource>> AbstractSampleSource::GetSubscribers()
{
    return subscribers;
}

double AbstractSampleSource::GetFrequency()
{
    return frequency;
}