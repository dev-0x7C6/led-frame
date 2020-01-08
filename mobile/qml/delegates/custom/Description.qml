import QtQuick 2.14
import QtQuick.Layouts 1.14

import "../../components/labels"
import "../../components/layouts"

ColumnLayout {
	property string name;
	property string detail;
	property string description;

	spacing: 0

	LayoutExpander {}

	LargeLabel { text: name; }
	MediumLabel { text: detail; }
	SmallLabel { text: description; }

	LayoutExpander {}
}
