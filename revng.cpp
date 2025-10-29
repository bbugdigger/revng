#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Module;
class Function;
class BasicBlock;

class BasicBlock {
public:
    BasicBlock(const std::string& name) : m_Name(name) {}
    
    //API
    void addSuccessor(BasicBlock* successor, const std::string& tag) {
        m_Successors[tag] = successor;
    }
    
    void removeSuccessor(const std::string& tag) {
        m_Successors.erase(tag);
    }
    
    const std::map<std::string, BasicBlock*>& getSuccessors() const {
        return m_Successors;
    }

    //getters
    const std::string& getName() const { return m_Name; }
private:
    std::string m_Name;
    std::map<std::string, BasicBlock*> m_Successors;
};

class Function {
public:
    Function(const std::string& name) : m_Name(name) {
        BasicBlock* entryBlock = new BasicBlock("entry");
        m_EntryBlock = entryBlock;
        m_BasicBlocks.push_back(entryBlock);
    }
    
    //API
    BasicBlock* createBasicBlock(const std::string& name) {
        auto block = new BasicBlock(name);
        m_BasicBlocks.push_back(block);
        return block;
    }

    void insertBasicBlock(BasicBlock* basicBlock) {
        if (basicBlock == nullptr)
            return;
        m_BasicBlocks.push_back(basicBlock);
    }

    void removeBasicBlock(const std::string& name) {
        auto it = std::find_if(m_BasicBlocks.begin(), m_BasicBlocks.end(),
            [&name](const BasicBlock* block) {
                return block->getName() == name;
            }
        );

        if (it != m_BasicBlocks.end()) { // this check is for when find_if returns .end(), and thats when it doesnt find a matching element
            if (*it == m_EntryBlock) { 
                // should we just delete the function at this point also? Functions should have at least one basic block
                m_EntryBlock = nullptr;
            }
            delete* it;
            m_BasicBlocks.erase(it);
        }
    }

    // graphviz grammar: https://graphviz.org/doc/info/lang.html
    // for testing out the generated output: http://www.webgraphviz.com/
    void toGraphViz() {
        std::cout << "digraph " << m_Name << " {\n";

        for (auto& block : m_BasicBlocks) {
            std::map<std::string, BasicBlock*> successors = block->getSuccessors();
            for (auto& [tag, successor] : successors) {
                std::cout << "  \"" << block->getName() << "\" -> \"" << successor->getName() << "\"";
                std::cout << " [label=\"" << tag << "\"]";
                std::cout << ";\n";
            }
        }

        std::cout << "}\n";
    }

    //getters
    const std::string& getName() const { return m_Name; }
    const std::vector<BasicBlock*>& getBasicBlocks() const { return m_BasicBlocks; }
    BasicBlock* getEntryBlock() const { return m_EntryBlock; }
private:
    std::string m_Name;
    std::vector<BasicBlock*> m_BasicBlocks;
    BasicBlock* m_EntryBlock;
};

class Module {
public:
    Module(const std::string& name) : m_Name(name) {}

    //API
    Function* createFunction(const std::string& name) {
        Function* function = new Function(name);
        m_Functions.push_back(function);
        return function;
    }

    void insertFunction(Function* function) {
        if (function == nullptr)
            return;
        m_Functions.push_back(function);
    }

    void removeFunction(const std::string& name) {
        auto it = std::find_if(m_Functions.begin(), m_Functions.end(),
            [&name](const Function* func) {
                return func->getName() == name;
            }
        );

        if (it != m_Functions.end()) {
            delete* it;
            m_Functions.erase(it);
        }
    }

    //getters
    const std::vector<Function*>& getFunctions() const { return m_Functions; }
private:
    std::string m_Name;
    std::vector<Function*> m_Functions;
};

void Part1() {
    std::cout << "Part1 Demo\n";
    
    auto module = new Module("example.cpp");
    
    auto mainFunc = module->createFunction("main");
    auto blablaFunc = module->createFunction("blabla");
    
    std::cout << "Module has " << module->getFunctions().size() << " functions\n";

    // Add basic blocks to main function
    mainFunc->createBasicBlock("some block");
    mainFunc->createBasicBlock("bla bla");
    mainFunc->createBasicBlock("to be deleted");
    mainFunc->createBasicBlock("exit");
    
    std::cout << "main function has " << mainFunc->getBasicBlocks().size() << " blocks\n";
    
    // Add basic blocks to blabla function
    blablaFunc->createBasicBlock("2nd block");
    blablaFunc->createBasicBlock("blabla");
    blablaFunc->createBasicBlock("exit");
    
    std::cout << "blabla function has " << blablaFunc->getBasicBlocks().size() << " blocks\n";
    
    // Remove a block
    mainFunc->removeBasicBlock("to be deleted");
    std::cout << "After removal, main has " << mainFunc->getBasicBlocks().size() << " blocks\n";
    
    delete module;
}

void Part2() {
    std::cout << "Part2 Demo\n";
    
    //Create module and function
    auto module = new Module("example.cpp");
    auto func = module->createFunction("test");
    
    //Create basic blocks from the control flow graph
    auto blockH = func->createBasicBlock("H");
    auto blockI = func->createBasicBlock("I");
    auto blockJ = func->createBasicBlock("J");
    
    //Add successors with tags
    auto entry = func->getEntryBlock();
    entry->addSuccessor(blockH, "true");
    entry->addSuccessor(blockI, "false");
    blockH->addSuccessor(blockJ, "");
    blockI->addSuccessor(blockJ, "");
    
    std::cout << "Generated GraphViz:\n\n";
    func->toGraphViz();
    
    delete module;
}

int main() {
    Part1();
    //Part2();
    
    return 0;
}
