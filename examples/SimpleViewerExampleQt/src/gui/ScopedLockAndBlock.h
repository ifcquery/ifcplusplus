#pragma once
#include <iostream>
#include <QtCore/QObject>
#include <QtWidgets/QWidget>

struct ScopedBlockCount
{
	std::map<QWidget*, int> m_widget_disabled_counter;
	std::map<bool*, int>	m_bool_to_true_counter;
	int m_inc = 0;
	std::map<QObject*, int> m_block_signals_counter;
	bool isBlocked(QObject* object)
	{
		if (m_block_signals_counter.find(object) == m_block_signals_counter.end())
		{
			return false;
		}
		return true;
	}
};

class ScopedBlockSignals
{
public:
	ScopedBlockSignals(QObject* block_sender, ScopedBlockCount& counter) : m_blocked_object(block_sender)
	{
		if (block_sender)
		{
			auto it = counter.m_block_signals_counter.find(block_sender);
			if (it == counter.m_block_signals_counter.end())
			{
				counter.m_block_signals_counter[block_sender] = 1;
			}
			else
			{
				int& counter_value = it->second;
				++counter_value;
			}
			block_sender->blockSignals(true);
			m_counter = &counter;
		}
	}

	~ScopedBlockSignals()
	{
		if (m_blocked_object)
		{
			if (m_counter)
			{
				auto it = m_counter->m_block_signals_counter.find(m_blocked_object);
				if (it == m_counter->m_block_signals_counter.end())
				{
					std::cout << "~ScopedBlockSignals: not found in m_block_signals_counter" << std::endl;
				}
				else
				{
					int& counter_value = it->second;
					--counter_value;
#ifdef _DEBUG
					if (counter_value < 0)
					{
						std::cout << "~ScopedBoolToTrue: counter_value < 0" << std::endl;
					}
#endif
					if (counter_value == 0)
					{
						m_blocked_object->blockSignals(false);
					}
				}
			}
		}
	}

	QObject* m_blocked_object;
	ScopedBlockCount* m_counter;
};

class ScopedIncrement
{
public:
	ScopedIncrement( ScopedBlockCount& counter)
	{
		m_counter = &counter;
		++m_counter->m_inc;
	}

	~ScopedIncrement()
	{
		if (m_counter)
		{
			--m_counter->m_inc;
		}
	}

	ScopedBlockCount* m_counter;
};

class ScopedBoolToTrue
{
public:
	ScopedBoolToTrue(bool& block_bool, ScopedBlockCount& counter)
	{
		m_scoped_bool_true = &block_bool;

		auto it = counter.m_bool_to_true_counter.find(m_scoped_bool_true);
		if (it == counter.m_bool_to_true_counter.end())
		{
			counter.m_bool_to_true_counter[m_scoped_bool_true] = 1;
		}
		else
		{
			int& counter_value = it->second;
			++counter_value;
		}
		block_bool = true;
		m_counter = &counter;
	}

	~ScopedBoolToTrue()
	{
		if (m_counter)
		{
			auto it = m_counter->m_bool_to_true_counter.find(m_scoped_bool_true);
			if (it == m_counter->m_bool_to_true_counter.end())
			{
				std::cout << "~ScopedBoolToTrue: not found in m_bool_to_true_counter" << std::endl;
			}
			else
			{
				int& counter_value = it->second;
				--counter_value;
#ifdef _DEBUG
				if (counter_value < 0)
				{
					std::cout << "~ScopedBoolToTrue: counter_value < 0" << std::endl;
				}
#endif
				if (counter_value == 0)
				{
					*m_scoped_bool_true = false;
				}
			}
		}
	}

	bool* m_scoped_bool_true;
	ScopedBlockCount* m_counter;
};


class ScopedWidgetDisabled
{
public:
	ScopedWidgetDisabled(QWidget* disable_widget, ScopedBlockCount& counter)
	{
		m_scoped_widget_disabled = disable_widget;
		if (m_scoped_widget_disabled)
		{
			auto it = counter.m_widget_disabled_counter.find(disable_widget);
			if (it == counter.m_widget_disabled_counter.end())
			{
				counter.m_widget_disabled_counter[disable_widget] = 1;
			}
			else
			{
				int& counter_value = it->second;
				++counter_value;
			}
			disable_widget->setDisabled(true);
			m_counter = &counter;
		}
	}

	~ScopedWidgetDisabled()
	{
		if (m_scoped_widget_disabled)
		{
			if (m_counter)
			{
				auto it = m_counter->m_widget_disabled_counter.find(m_scoped_widget_disabled);
				if (it == m_counter->m_widget_disabled_counter.end())
				{
					std::cout << "~ScopedWidgetDisabled: not found in m_widget_disabled_counter" << std::endl;
				}
				else
				{
					int& counter_value = it->second;
					--counter_value;
#ifdef _DEBUG
					if (counter_value < 0)
					{
						std::cout << "~ScopedWidgetDisabled: counter_value < 0" << std::endl;
					}
#endif
					if (counter_value == 0)
					{
						m_scoped_widget_disabled->setDisabled(false);
					}
				}
			}
		}
	}

	QWidget* m_scoped_widget_disabled;
	ScopedBlockCount* m_counter;
};
