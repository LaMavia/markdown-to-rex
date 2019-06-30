# markdown-to-rex

## Table of Contents:

1. [Installation](#inst)
1. [Usage](#us)
   1. [Markdown](#us-mark)
   1. [JSX Extended Markdown](#us-mark-ext)
   1. [Other options](#us-ops)

## <div id="inst"> Installation

`npm i markdown-to-rex`

## <div id="us"> Usage

### <div id="us-mark"> Usage > Markdown

Just as the package [markdown-to-jsx](https://www.npmjs.com/package/markdown-to-jsx), you can you this one to parse markdown in ReasonML. Although the example below uses interpolated strings, you can still use the good old `"Double quotes string :)"`.

```reasonml
[@react.component]
let make = () =>
  <Markdown>
    {|
      # Hello
      ## there
      ### mate
    |}
  </Markdown>
```

Renders
```html
<div>
  <h1 id="hello">Hello</h1>
  <h2 id="there">there</h2>
  <h3 id="mate">mate</h3>
</div>
```

### <div id="us-mark-ext"> Usage > JSX Extended Markdown

You can also use custom ReasonReact components, but there are some caveats...
Firstyl, your component can't be passed directly as a prop (because of [ReasonReact design](https://reasonml.github.io/reason-react/docs/en/component-as-prop)).<br/>
Secondly, your component's props (arguments) can be undefined, so you have to handle that.<br/>
Here's a way to tackle these problems:
```reasonml
// Here's our component. As you can see, it's nothing special.
module FancyText {
  [@react.component]
  let make = (~prefix=?, ~children: React.element=?) => {
    <div>
      // Helpers.vor does basically: (val, def) => val ? val : def
      {(prefix->Helpers.vor("") ++ " Fancy Text: ")->React.string}
      <strong> children </strong>
    </div>;
  };
}

// Here's where the magic begins...
// This is our higher-order function which we can pass as a property
// It also needs to define all it's props
let fancy_text:
  Markdown.props({
    .
    children: React.element,
    prefix: string,
  }) =
  p =>
    switch (p) {
    | Some(p) => <FancyText prefix=p##prefix> {p##children} </FancyText>
    | None => <FancyText />
    };

// After all of this hard work we can get to the good stuf - actually using out component

ReactDOMRe.renderToElementWithId(
  <Markdown
    overrides=[|("FancyText", fancy_text |> Markdown.override_of_component)|]>
    {|
      # Title
      <FancyText prefix="This is my "> it's fancy</FancyText>
    |}
  </Markdown>,
  "root",
);
```
To our `Markdown` component, we're passing all of our overrides in a form of 
```reasonml
array((
  string /*tag name*/, 
  override /*higher-order component passed through Markdown.override_of_component*/
))
```

### <div id="us-ops"> Usage > Other Options
You can also use the `forceInline` and `forceBlock` options from the original package.
```reasonml
// They both default ot false (as stated in the original documentation)
<Markdown forceInline=true forceBlock=false>
    {|
      # Hello
      lorem ipsum dolor sit amet
    |}
  </Markdown>
```