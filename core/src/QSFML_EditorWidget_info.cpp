#include "QSFML_EditorWidget_info.h"

/// USER_SECTION_START 1

/// USER_SECTION_END

#include <iostream>

#if (defined(QT_CORE_LIB) && defined(QT_WIDGETS_LIB))
	#include <QWidget>
	#include <QLabel>
	#include <QVBoxLayout>
	#include <vector>
	#define QT_WIDGETS_AVAILABLE
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

namespace QSFML
{
/// USER_SECTION_START 3

/// USER_SECTION_END

	// compare two versions
	bool LibraryInfo::Version::operator<(const Version& other) const
	{
		if (major < other.major)
			return true;
		if (major > other.major)
			return false;
		if (minor < other.minor)
			return true;
		if (minor > other.minor)
			return false;
		if (patch < other.patch)
			return true;
		return false;
	}

	bool LibraryInfo::Version::operator==(const Version& other) const
	{
		return major == other.major && minor == other.minor && patch == other.patch;
	}
	bool LibraryInfo::Version::operator!=(const Version& other) const
	{
		return !(*this == other);
	}
	bool LibraryInfo::Version::operator>(const Version& other) const
	{
		return !(*this < other) && !(*this == other);
	}
	bool LibraryInfo::Version::operator<=(const Version& other) const
	{
		return *this < other || *this == other;
	}
	bool LibraryInfo::Version::operator>=(const Version& other) const
	{
		return *this > other || *this == other;
	}
	std::string LibraryInfo::Version::toString() const
	{
		// fornmat: XX.YY.ZZZZ
		// Add leading digits if needed
		std::string majorStr = std::to_string(major);
		std::string minorStr = std::to_string(minor);
		std::string patchStr = std::to_string(patch);
		if (majorStr.size() < 2)
			majorStr = "0" + majorStr;
		if (minorStr.size() < 2)
			minorStr = "0" + minorStr;
		if (patchStr.size() < 4)
		{
			while (patchStr.size() < 4)
				patchStr = "0" + patchStr;
		}
		return majorStr + "." + minorStr + "." + patchStr;
	}

	void LibraryInfo::printInfo()
	{
		printInfo(std::cout);
	}
	void LibraryInfo::printInfo(std::ostream& stream)
	{
		std::stringstream ss;
		ss << "Library Name: " << name << "\n"
			<< "Author: " << author << "\n"
			<< "Email: " << email << "\n"
			<< "Website: " << website << "\n"
			<< "License: " << license << "\n"
			<< "Version: " << version.toString() << "\n"
			<< "Compilation Date: " << compilationDate << "\n"
			<< "Compilation Time: " << compilationTime << "\n";

		stream << ss.str();
	}
	std::string LibraryInfo::getInfoStr()
	{
		std::stringstream ss;
		LibraryInfo::printInfo(ss);
		return ss.str();
	}

#ifdef QT_WIDGETS_AVAILABLE
	void addRow(const QString& labelText, const QString& valueText, QGridLayout* layout, int row) {
		QLabel* label = new QLabel(labelText);
		label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		layout->addWidget(label, row, 0);

		QLabel* valueLabel = new QLabel(valueText);
		layout->addWidget(valueLabel, row, 1);
	}
	QWidget* LibraryInfo::createInfoWidget(QWidget* parent)
	{
		QWidget* widget = new QWidget(parent);
		QGridLayout* layout = new QGridLayout(widget);
		
		struct Pair 
		{
			std::string label;
			std::string value;
		};
		std::vector< Pair> pairs = {
			{"Library Name:", name},
			{"Author:", author},
			{"Email:", email},
			{"Website:", website},
			{"License:", license},
			{"Version:", version.toString()},
			{"Compilation Date:", compilationDate},
			{"Compilation Time:", compilationTime},
			{"Build Type:", buildTypeStr},
		};
		int rowCount = 0;
		for (const auto& pair : pairs) {
			addRow(QString::fromStdString(pair.label), QString::fromStdString(pair.value), layout, rowCount++);
		}

		widget->setLayout(layout);
		return widget;
	}
#else
	QWidget* LibraryInfo::createInfoWidget(QWidget* parent)
	{
		QSFML_UNUSED(parent);
		return nullptr;
	}
#endif




/// USER_SECTION_START 4

/// USER_SECTION_END
}

/// USER_SECTION_START 5

/// USER_SECTION_END