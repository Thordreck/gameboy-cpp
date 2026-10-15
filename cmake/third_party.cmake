function(add_third_party_module name url license)
    set(entry "{}")
    string(JSON entry SET ${entry} name \"${name}\")
    string(JSON entry SET ${entry} url \"${url}\")
    string(JSON entry SET ${entry} license \"${license}\")

    message("Json entry: ${entry}")
endfunction()
