#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace TT {
	class Writer;

	class Elem {
	public:
		virtual ~Elem() {}
		virtual void write(Writer &w) = 0;
	};

	class Word;
	using Word_Ptr = std::unique_ptr<Word>;

	class Word: public Elem {
		const std::string word_;
	public:
		Word(const std::string &word):
			word_ { word }
		{ }
		static Word_Ptr create(const std::string &word) {
			return std::make_unique<Word>(word);
		}
		const std::string &word() const {
			return word_;
		}
		void write(Writer &w) override;
	};

	using Elem_Ptr = std::unique_ptr<Elem>;
	class List;
	using List_Ptr = std::unique_ptr<List>;
	
	class List: public Elem {
		const Word command_;
		using Container_ = std::vector<Elem_Ptr>;
		Container_ arguments_;
	public:
		List(const Word &command):
			command_ { command }
		{ }
		static List_Ptr create(const Word &command) {
			return std::make_unique<List>(command);
		}
		static List_Ptr create(const std::string &command) {
			return create(Word { command });
		}
		const Word &command() const {
			return command_;
		}
		void push_back(Elem_Ptr &&elem) {
			arguments_.emplace_back(std::move(elem));
		}
		Container_::const_iterator begin() const {
			return arguments_.begin();
		}
		Container_::const_iterator end() const {
			return arguments_.end();
		}
		void write(Writer &w) override;
	};

	class Writer {
		std::ostream &out_;
		int col_ { 0 };
		int indent_ { 0 };
		int max_col_ { 80 };
		int spaces_per_tab_ { 4 };
		bool new_line_ { true };
	public:
		Writer(std::ostream &out = std::cout): out_ { out } { }
		~Writer() {
			if (! new_line_) { out_ << '\n'; }
		}
		void new_line() {
			out_ << '\n';
			col_ = 0;
			for (int i { indent_ }; i > 0; --i) {
				out_ << '\t';
				col_ += spaces_per_tab_;
			}
			new_line_ = true;
		}
		void write(const std::string &wrd) {
			if (new_line_) {
			} else if (col_ + 1 + wrd.size() < max_col_) {
				out_ << ' '; ++col_;
				new_line_ = false;
			} else {
				new_line();
			}
			if (wrd.size()) {
				out_ << wrd;
				col_ += wrd.size();
				new_line_ = false;
			}
		}
		void indent() { ++indent_; }
		void outdent() { --indent_; }
	};

	void Word::write(Writer &w) {
		w.write(word_);
	}
	
	void List::write(Writer &w) {
		if (begin() == end()) {
			w.write("{" + command_.word() + "}");
		} else {
			w.write("{" + command_.word());
			w.indent();
			w.new_line();
			for (const auto &arg : arguments_) {
				arg->write(w);
			}
			w.outdent();
			w.new_line();
			w.write("}");
		}
	}
}
