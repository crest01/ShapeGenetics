


#include <cstdlib>
#include <algorithm>
#include <iostream>

#include <CUDA/device.h>

#include <CUPTI/error.h>
#include <CUPTI/device.h>
#include <CUPTI/event.h>
#include <CUPTI/metric.h>


namespace
{
	const char* translateEventCategory(CUpti_EventCategory category)
	{
		switch (category)
		{
			case CUPTI_EVENT_CATEGORY_INSTRUCTION:
				return "instruction";

			case CUPTI_EVENT_CATEGORY_MEMORY:
				return "memory";

			case CUPTI_EVENT_CATEGORY_CACHE:
				return "cache";

			case CUPTI_EVENT_CATEGORY_PROFILE_TRIGGER:
				return "profile trigger";
		}
		return "invalid CUpti_EventCategory";
	}

	const char* translateEventCollectionMethod(CUpti_EventCollectionMethod method)
	{
		switch (method)
		{
			case CUPTI_EVENT_COLLECTION_METHOD_PM:
				return "global hardware counter";

			case CUPTI_EVENT_COLLECTION_METHOD_SM:
				return "per MP hardware counter";

			case CUPTI_EVENT_COLLECTION_METHOD_INSTRUMENTED:
				return "software instrumentation";
		}
		return "invalid CUpti_EventCollectionMethod";
	}

	const char* translateMetricCategory(CUpti_MetricCategory category)
	{
		switch (category)
		{
			case CUPTI_METRIC_CATEGORY_MEMORY:
				return "memory";

			case CUPTI_METRIC_CATEGORY_INSTRUCTION:
				return "instruction";

			case CUPTI_METRIC_CATEGORY_MULTIPROCESSOR:
				return "multiprocessor";

			case CUPTI_METRIC_CATEGORY_CACHE:
				return "cache";

			case CUPTI_METRIC_CATEGORY_TEXTURE:
				return "texture";
		}
		return "invalid CUpti_MetricCategory";
	}

	const char* translateMetricValueKind(CUpti_MetricValueKind value_kind)
	{
		switch (value_kind)
		{
			case CUPTI_METRIC_VALUE_KIND_DOUBLE:
				return "double";

			case CUPTI_METRIC_VALUE_KIND_UINT64:
				return "uint64";

			case CUPTI_METRIC_VALUE_KIND_PERCENT:
				return "percent";

			case CUPTI_METRIC_VALUE_KIND_THROUGHPUT:
				return "throughput";

			case CUPTI_METRIC_VALUE_KIND_INT64:
				return "int64";

			case CUPTI_METRIC_VALUE_KIND_UTILIZATION_LEVEL:
				return "utilization";
		}
		return "invalid CUpti_MetricValueKind";
	}

	const char* translateMetricEvaluationMode(CUpti_MetricEvaluationMode mode)
	{
		switch (mode)
		{
			case CUPTI_METRIC_EVALUATION_MODE_PER_INSTANCE:
				return "per instance";

			case CUPTI_METRIC_EVALUATION_MODE_AGGREGATE:
				return "aggregate";

			case CUPTI_METRIC_EVALUATION_MODE_PER_INSTANCE | CUPTI_METRIC_EVALUATION_MODE_AGGREGATE:
				return "any";
		}
		return "invalid CUpti_MetricEvaluationMode";
	}
}

void printCUPTISetup(CUdevice device)
{
	std::cout << CU::getDeviceName(device) << '\n';

	auto global_memory_bandwidth = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_GLOBAL_MEMORY_BANDWIDTH>(device);
	auto instructions_per_cycle = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_INSTRUCTION_PER_CYCLE>(device);
	auto single_precision_instruction_throughput = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_INSTRUCTION_THROUGHPUT_SINGLE_PRECISION>(device);
	auto max_frame_buffers = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_MAX_FRAME_BUFFERS>(device);
	auto pcie_link_rate = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_PCIE_LINK_RATE>(device);
	auto pcie_link_width = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_PCIE_LINK_WIDTH>(device);
	auto pcie_generation = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_PCIE_GEN>(device);
	auto device_class = CUPTI::getDeviceAttribute<CUPTI_DEVICE_ATTR_DEVICE_CLASS>(device);

	std::cout << "\n-- events --\n";

	auto event_domains = CUPTI::enumEventDomains(device);

	for (auto d = std::begin(event_domains); d != std::end(event_domains); ++d)
	{
		auto domain_name = CUPTI::getEventDomainAttribute<CUPTI_EVENT_DOMAIN_ATTR_NAME>(*d);
		auto domain_instance_count = CUPTI::getEventDomainAttribute<CUPTI_EVENT_DOMAIN_ATTR_INSTANCE_COUNT>(device, *d);
		auto domain_total_instance_count = CUPTI::getEventDomainAttribute<CUPTI_EVENT_DOMAIN_ATTR_TOTAL_INSTANCE_COUNT>(device, *d);
		auto domain_collection_method = CUPTI::getEventDomainAttribute<CUPTI_EVENT_DOMAIN_ATTR_COLLECTION_METHOD>(*d);

		std::cout << domain_name << " ("
		          << domain_instance_count << " profileable instances, "
		          << domain_total_instance_count << " instances total, collection method: "
		          << translateEventCollectionMethod(domain_collection_method) << ")\n";

		auto events = CUPTI::enumEventsInDomain(*d);

		std::stable_sort(std::begin(events), std::end(events), [](CUpti_EventID a, CUpti_EventID b) -> bool
		{
			auto ca = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_CATEGORY>(a);
			auto cb = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_CATEGORY>(b);
			return ca < cb;
		});

		CUpti_EventCategory prev_category = CUPTI_EVENT_CATEGORY_FORCE_INT;
		for (auto e = std::begin(events); e != std::end(events); ++e)
		{
			auto event_name = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_NAME>(*e);
			auto event_description = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_LONG_DESCRIPTION>(*e);
			auto category = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_CATEGORY>(*e);

			if (category != prev_category)
			{
				std::cout << "  " << translateEventCategory(category) << '\n';
				prev_category = category;
			}

			std::cout << "    " << event_name << "  "
			          << event_description << '\n';
		}
	}

	std::cout << "\n-- metrics --\n";

	auto metrics = CUPTI::enumMetrics(device);

	std::stable_sort(std::begin(metrics), std::end(metrics), [](CUpti_MetricID a, CUpti_MetricID b) -> bool
	{
		auto ca = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_CATEGORY>(a);
		auto cb = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_CATEGORY>(b);
		return ca < cb;
	});

	CUpti_MetricCategory prev_category = CUPTI_METRIC_CATEGORY_FORCE_INT;
	for (auto m = std::begin(metrics); m != std::end(metrics); ++m)
	{
		auto metric_name = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_NAME>(*m);
		auto description = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_LONG_DESCRIPTION>(*m);
		auto category = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_CATEGORY>(*m);
		auto value_kind = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_VALUE_KIND>(*m);
		auto evaluation_mode = CUPTI::getMetricAttribute<CUPTI_METRIC_ATTR_EVALUATION_MODE>(*m);

		if (category != prev_category)
		{
			std::cout << translateMetricCategory(category) << '\n';
			prev_category = category;
		}

		std::cout << "  " << metric_name << " ("
		          << translateMetricValueKind(value_kind) << ", evaluation mode: "
		          << translateMetricEvaluationMode(evaluation_mode) << ")\n    "
		          << description << "\n";

		auto events_in_metric = CUPTI::enumEventsInMetric(*m);
		for (auto e = std::begin(events_in_metric); e != std::end(events_in_metric); ++e)
		{
			auto event_name = CUPTI::getEventAttribute<CUPTI_EVENT_ATTR_NAME>(*e);
			std::cout << "    " << event_name << '\n';
		}
	}
}

int main(int argc, char* argv[])
{
	try
	{
		checkError(cuInit(0U));
		int device_ordinal;

		if (argc == 2)
			device_ordinal = std::atoi(argv[1]);
		else if (argc == 1)
			device_ordinal = 0;
		else
		{
			std::cout << "usage: cupti_info [<device-ordinal>]\n";
			return -1;
		}

		auto device = CU::getDevice(device_ordinal);

		printCUPTISetup(device);
	}
	catch (std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		return -128;
	}

	return 0;
}
