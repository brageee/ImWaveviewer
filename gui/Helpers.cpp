#include "Helpers.h"

namespace gui
{
    bool InputSampleIndex(int &sampleIndex, int &samplesPerRow, int numSamples)
    {
        bool res = false;
        if(ImGui::InputInt("##currentsampleindex", &sampleIndex))
        {
           res = true;
           if(sampleIndex < 0)
            sampleIndex = 0;
           if(sampleIndex+samplesPerRow >= numSamples)
                samplesPerRow = numSamples-sampleIndex;            
        }
        return res;
    }

    bool InputSamplesPerRow(int &sampleIndex, int &samplesPerRow, int numSamples)
    {
        bool res = false;
        if(ImGui::InputInt("##numsamplesperrow", &samplesPerRow))
        {
            res = true;
            if(sampleIndex+samplesPerRow >= numSamples)
                sampleIndex = numSamples-samplesPerRow;
            if(sampleIndex < 0)
            {
                sampleIndex = 0;
                samplesPerRow = numSamples;
            }
            if(samplesPerRow<1)
                samplesPerRow = 1;                             
        }
        return res;
    }

    bool InputStrAndEvaluateToInt(const char *label, std::string &value)
    {
        bool res = false;
        std::string backup = value;        
        bool evaluate = false;
        double dresult = 0.0;        
        if(ImGui::InputText(label, &value))        
        {
            evaluate = true;
            res = true;
        }
        if(evaluate)
        {            
            expression_t expression;
            parser_t parser;

            if (parser.compile(value,expression))
            {
                dresult= expression.value();
                //std::cout << "Result was " << dresult << std::endl;
                int result = static_cast<int>(dresult);
                value = std::to_string(result);
                //printf("Result: %19.15 \n",dresult);
            }
            else
            {
                //printf("Error in expression. \n");
                value = backup;
            }                
        }        
        return res;
    }

    bool InputStrAndEvaluateToDouble(const char *label, std::string &value)
    {
        bool res = false;
        std::string backup = value;        
        bool evaluate = false;
        double result = 0.0;        
        if(ImGui::InputText(label, &value))        
        {
            evaluate = true;
            res = true;
        }
        if(evaluate)
        {            
            expression_t expression;
            parser_t parser;

            if (parser.compile(value,expression))
            {
                result= expression.value();                
                value = std::to_string(result);             
            }
            else
            {                
                value = backup;
            }                
        }        
        return res;
    }

    bool LoadSettings(Settings &settings, std::string filename)
    {            
        //std::cout << "Filename: " << filename << std::endl;
        std::ifstream file(filename);    
        std::map<std::string, std::string> settingsMap;    

        if(file) //config file exists
        {
            file.seekg(0,std::ios::end);
            std::streampos length = file.tellg();
            file.seekg(0,std::ios::beg);

            // Read in to buffer
            std::vector<char>       buffer(length);
            file.read(&buffer[0],length);
            file.close();

            // Parse buffer to stream
            std::stringstream       localStream;
            localStream.rdbuf()->pubsetbuf(&buffer[0],length);

            std::string line;
            while( std::getline(localStream, line) )
            {
                std::istringstream is_line(line);
                std::string key;
                if( std::getline(is_line, key, '=') ) //gets the part before "="
                {
                    std::string value;
                    if( std::getline(is_line, value) ) //gets the part after "="
                    {
                        //std::cout << "key " << key << ", value " << value << std::endl;
                        settingsMap.insert({key,value});
                    }                        
                    //store_line(key, value);
                }
            }            
        } else
        {
            std::cout << "Could not read config file" << std::endl;
            //Create config file
            std::vector<std::string> lines;
            CreateConfig(lines, settingsMap);
            
            //Write all lines to file
            std::ofstream file(filename, std::ios::out | std::ios::trunc);
            for(const auto& i : lines) {
                file << i << std::endl;
            }
            file.close();            
        }

        //Go through settings and add to settings struct
        for (auto p : settingsMap)
        {
            //std::cout << "p1: " << p.first << ", p2: " << p.second << std::endl;
            if(p.first.compare("path") == 0)
            {
                std::string localPath = "";
                //Check if we need to parse environment variable                
                if(p.second.find("$") != std::string::npos)
                {
                    // We assume $ is first character
                    p.second.erase(0,1);                    
                    char * val = std::getenv(p.second.c_str());
                    if (val == NULL)
                        localPath = "";
                    else
                        localPath = std::string(val);
                }
                else
                    localPath = p.second;
                //std::cout << "Path: " << localPath << std::endl;
                settings.defaultPath = localPath;
            } else if (p.first.compare("updatePath") == 0)
            {
                if(p.second.compare("True") == 0)
                    settings.updatePath = true;
                else
                    settings.updatePath = false;
            }
        }
        
        return true;

    }

    bool SetSettingsPath(std::string path, std::string filename)
    {
        //Remove filename from path
        std::size_t lastBackSlash = path.find_last_of("/\\");
        std::string localPath =  path.substr(0,lastBackSlash);
        
        //Open file and find path line
        std::ifstream file(filename);    
        if(file) //config file exists
        {
            file.seekg(0,std::ios::end);
            std::streampos length = file.tellg();
            file.seekg(0,std::ios::beg);

            // Read in to buffer
            std::vector<char>       buffer(length);
            file.read(&buffer[0],length);
            file.close();

            // Parse buffer to stream
            std::stringstream       localStream;
            localStream.rdbuf()->pubsetbuf(&buffer[0],length);

            std::string line;
            std::vector<std::string> lines;
            while( std::getline(localStream, line) )
            {
                std::istringstream is_line(line);
                std::string key;
                if( std::getline(is_line, key, '=') ) //gets the part before "="
                {
                    if(key.compare("path") == 0)
                    {
                        line = "path="+localPath;
                    }                                            
                }
                lines.push_back(line);
            }
            
            //Write all lines to file
            std::ofstream file(filename, std::ios::out | std::ios::trunc);
            for(const auto& i : lines) {
                file << i << std::endl;
            }
            file.close();
        }
        else
        {
            std::cout << "Could not update config file" << std::endl;
        }
        return true;
    }
    
    bool CreateConfig(std::vector<std::string> &lines, std::map<std::string, std::string> &settingsMap)
    {
        lines.clear();
        lines.push_back("[Directory settings]");
        lines.push_back("path=$HOME");
        lines.push_back("updatePath=False");
        //Insert into map
        settingsMap.clear();
        settingsMap.insert({"path","$HOME"});
        settingsMap.insert({"updatePath","false"});

        return true;
    }

    std::string formatSIValue(double value, unsigned int decimals)
    {
        std::map<int, std::string> prefixes =
        {
            {  9,   "G" },
            {  6,   "M" },
            {  3,   "k" },
            {  0,   ""  },
            { -3,   "m" },
            { -6,   "u" },
            { -9,   "n" },
        };

        bool isNegative = (value < 0.0f) ? true: false;

        if(isNegative) value = value * -1.0f;

        int power = 0;
        while (value < 1.0f && power > -9)
        {
            value *= 1e3;
            power -= 3;
        }
        while (value >= 1e3 && power < 9)
        {
            value *= 1e-3;
            power += 3;
        }

        if(isNegative) value = value * -1.0f;
        
        std::ostringstream streamObj;
        // Set Fixed -Point Notation
        streamObj << std::fixed;
        // Set precision
        streamObj << std::setprecision(decimals);
        //Add double to stream
        streamObj << value;

        return streamObj.str() + prefixes[power];
    }


    bool isNumber(const std::string& s)
    {
        try
        {
            std::stod(s);
        }
        catch(...)
        {
            return false;
        }
        return true;
    }

    bool ParseTakeSkipString(TakeSkipVars &vars)
    {
        /* Keywords
        t = take
        s = skip
        d = insert 0
        ()X = do content of () X times
        Example:
        (t1s1)5s5 = take 1, skip 1 sample 5 times and then skip 5 samples and repeat
        */
        bool result = true;        
        std::cout << vars.str << std::endl; 
        std::vector<std::string> cmds;
        std::vector<int> reps;
        std::string str = vars.str;
        ExtractCommands(str, cmds, reps);
        vars.pattern = CommandsToPattern(cmds, reps);
        //Convert TakeSkipParserVar to take skip pattern

        return result;
    }

    void ExtractCommands(std::string &tmp, std::vector<std::string> &cmds, std::vector<int> &reps)
    {        
        int pos = tmp.find_first_of("(");
        while (pos != std::string::npos)
        {
            //Check if there is command preceding (
            if(pos != 0)
            {
                //Add this command
                std::string cmd = tmp.substr(0,pos);
                cmds.push_back(cmd);
                reps.push_back(1);
                //std::cout << "Adding " << cmd << ", repeated " << 1 << " time" << std::endl;
                //Update tmp string and pos
                std::string rest = tmp.substr(pos, std::string::npos);
                tmp = rest;
                pos = tmp.find_first_of("(");
            } else
            {
                //Find next ), add substring to TakeSkipParserVar and remove that part from tmp
                int posNext = tmp.find(")");
                if(posNext != std::string::npos)
                {
                    std::string cmd = tmp.substr(pos+1, posNext-1);
                    std::string rest = tmp.substr(posNext+1,std::string::npos);
                    //std::cout << "Remainder " << rest << std::endl;
                    //Extract repetition value
                    int rep = 1;
                    pos = FindDelimiter(rest);                    
                    rep = std::atoi(rest.substr(0,pos).c_str());
                    if(rep == 0)
                        rep = 1;
                    cmds.push_back(cmd);
                    reps.push_back(rep);
                    //std::cout << "Adding " << cmd << ", repeated " << rep << " times" << std::endl;
                    tmp = rest.substr(pos, std::string::npos);
                    pos = tmp.find_first_of("(");                           
                } else
                {
                    //Return false, could not find corresponding parenthesis
                }           
            }
        }
        if(tmp.length()>0)
        {
            cmds.push_back(tmp);
            reps.push_back(1);
        }
    }

    std::vector<int> CommandsToPattern(std::vector<std::string> &cmds, std::vector<int> &reps)
    {
        std::vector<int> pattern;
        int value = 0;
        int rep = 1;
        for (size_t i = 0; i < cmds.size(); i++)
        {
            std::string cmd = cmds[i];
            int pos = 0;
            std::vector<int> tmpPattern;
            while (pos < cmd.length())
            {
                if(cmd[pos] == 't')
                    value = 1;
                else if (cmd[pos] == 's')
                    value = 0;
                else if (cmd[pos] == 'd')
                    value = 2;
                int delPos = FindDelimiter(cmd.substr(pos+1,std::string::npos));
                rep = std::atoi(cmd.substr(pos+1,delPos).c_str());
                if(rep<1)
                    std::cout << "Error parsing command" << std::endl;
                else
                {
                    for (size_t j = 0; j < rep; j++)
                    {
                        tmpPattern.push_back(value);
                    }                
                }
                pos += delPos+1;
            }
            for (size_t j = 0; j < reps[i]; j++)
            {
                for (auto v : tmpPattern)
                {
                    pattern.push_back(v);
                }            
            }        
        }
        return pattern;
    }

    int FindDelimiter(std::string str)
    {
        int pos = str.length();
        int pos1 = str.find_first_of("(");
        int pos2 = str.find_first_of("t");
        int pos3 = str.find_first_of("s");
        int pos4 = str.find_first_of("d");                                                   
        if(pos1 > 0)    
            pos = pos1;
        if(pos2 < pos && pos2 > 0)
            pos = pos2;
        if(pos3 < pos && pos3 > 0)
            pos = pos3;
        if(pos3 < pos && pos4 > 0)
            pos = pos4;
        return pos;
    }
} // namespace gui
