#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <vissh/vssh.h>
#include <sstream>
#include <algorithm>


class VirtualDaemon {

};


class Tab {
private:
	std::string _pwd = "/"; // Path of the current working directory
	std::string _contentFilter = ""; // Filter for content for Imgui
	std::vector<FileSystemItem> _content;


public:
	std::string normalizePath(const std::string& path) const;

	void setContent(const std::vector<FileSystemItem>& newContent) {
		_content = newContent;
	}

	void setPwd(const std::string& newPwd) {
		_pwd = normalizePath(newPwd);
	}

	const std::vector<FileSystemItem>& getContent() const {
		return _content;
	}

	const std::string& getPwd() const {
		return _pwd;
	}
};

class VirtualEnvironment
{
private:

	std::string _pwd; // Path of the current working directory

	std::unordered_map<std::string, Tab> _tabs; // map holding tables of strings, like a spreadsheet with rows and columns.

public:

	Tab& getTab(const std::string& tabID);

	const std::unordered_map<std::string, Tab>& getTabs() const { return _tabs;}
	

	bool removeTab(const std::string& tabID);
	void setTab(std::string tabID, std::vector<FileSystemItem> content);

	void createNewTab(const std::string& tabID);

	VirtualEnvironment();
	~VirtualEnvironment();

};



