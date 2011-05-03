.. index::
   pair: Forms; View

テンプレート内でフォームを使用する
==================================

.. 翻訳を更新するまで以下を表示
.. caution::

    このドキュメントの内容は古い内容です。最新の内容は公式の英語ドキュメントをご確認ください。

Symfony2 の :doc:`フォーム </book/forms/overview>` はフィールドでできています。フィールドはフォームの意味を記述しており、エンドユーザに対する表現を記述しているわけではありません。これは、フォームを HTML に関連付ける必要がないことを意味しています。代わりに、それぞれのフォームフィールドが思ったとおりに表示されることは、ウェブデザイナーの責任になるということです。ただし、Symfony2 は PHP 向けのヘルパーと Twig テンプレートを提供することで、フォームの統合とカスタマイズを簡単にしています。

フォームを「手動で」表示する
----------------------------

Symfony2 のラッパーおよび Symfony2 がどのように簡単にかつ確実ですばやくフォームの表示ができるよう手助けするのかについて詳しく見ていく前に、見えないところでは何も特別なことが起きているわけではないということを知っておく必要があります。Symfony2 のフォームを表示させるため、どんな HTML も使うことができるのです。

.. code-block:: html

    <form action="#" method="post">
        <input type="text" name="name" />

        <input type="submit" />
    </form>

バリデーションのエラーがある場合、問題をすばやく修正するため、エラーを表示し、フィールドに送信された値を入れるべきでしょう。これにはフォーム専用のメソッドを使うだけです。

.. configuration-block::

    .. code-block:: html+jinja

        <form action="#" method="post">
            <ul>
                {% for error in form.name.errors %}
                    <li>{{ error.0 }}</li>
                {% endfor %}
            </ul>
            <input type="text" name="name" value="{{ form.name.data }}" />

            <input type="submit" />
        </form>

    .. code-block:: html+php

        <form action="#" method="post">
            <ul>
                <?php foreach ($form['name']->getErrors() as $error): ?>
                    <li><?php echo $error[0] ?></li>
                <?php endforeach; ?>
            </ul>
            <input type="text" name="name" value="<?php $form['name']->getData() ?>" />

            <input type="submit" />
        </form>

Symfony2 のヘルパーは、テンプレートを短くしたり、フォームのレイアウトを簡単にカスタマイズできるようにしたり、国際化をサポートしたり、CSRF から守ったり、ファイルをアップロードできたりといったことが簡単にできてしまいます。以降のセクションで、これらについて説明していきます。

フォームを表示する
------------------

フォームのグローバルな構造 (フォームタグや送信ボタンなど) はフォームインスタンスで定義されているわけではないので、使いたい HTML コードを自由に使うことができます。単純なフォームのテンプレートは以下のように読み込みます。

.. code-block:: html

    <form action="#" method="post">
        <!-- Display the form fields -->

        <input type="submit" />
    </form>

グローバルなフォームの構造以外にも、グローバルなエラーや隠しフィールドを表示するための方法も必要です。Symfony2 はこの役割を果たすヘルパーを用意しています。Twig テンプレートにおいてこれらのヘルパーは、フォームやフォームのフィールドに適用できるグローバルな関数として実装されています。PHP テンプレートにおいては「フォーム」ヘルパーが、フォームやフォームのフィールドをパラメータとして受け入れるパブリックメソッドを通じて同じ機能を提供しています。

.. configuration-block::

    .. code-block:: html+jinja

        <form action="#" method="post">
            {{ form_errors(form) }}

            <!-- フォームのフィールドを表示する -->

            {{ form_hidden(form) }}
            <input type="submit" />
        </form>

    .. code-block:: html+php

        <form action="#" method="post">
            <?php echo $view['form']->errors($form) ?>

            <!-- フォームのフィールドを表示する -->

            <?php echo $view['form']->hidden($form) ?>

            <input type="submit" />
        </form>

.. note::

    見ての通り、Twig の関数は「form\_ 」で始まります。「フォーム」ヘルパーのメソッドと異なり、これらの関数はグローバルであり、名前が重複しやすいので注意してください。

.. tip::

    デフォルトでは、 ``errors`` ヘルパーは ``<ul>`` リストを生成します。これは、このドキュメントの後に出てくるように、簡単にカスタマイズすることができます

最後に重要なこととして、ファイル入力を含むフォームは ``enctype`` 属性を持つ必要があります。そのようなフォームをレンダリングする際は ``enctype`` ヘルパーを使用しましょう。

.. configuration-block::

    .. code-block:: html+jinja

        <form action="#" {{ form_enctype(form) }} method="post">

    .. code-block:: html+php

        <form action="#" <?php echo $view['form']->enctype($form) ?> method="post">

フィールドを表示する
--------------------

フォームのフィールドへのアクセスは、Symfony2 のフォームが配列として動作するのと同じくらい簡単です。

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.title }}

        {# グループ user 内に入れ子になったフィールド first_name にアクセス #}
        {{ form.user.first_name }}

    .. code-block:: html+php

        <?php $form['title'] ?>

        <!-- グループ user 内に入れ子になったフィールド first_name にアクセス -->
        <?php $form['user']['first_name'] ?>

それぞれのフィールドが Field インスタンスであることから、上に示したように表示することはできません。ヘルパーを代わりに使用してください。

``render`` ヘルパーは、フィールドの HTML 表現をレンダリングします。

.. configuration-block::

    .. code-block:: jinja

        {{ form_field(form.title) }}

    .. code-block:: html+php

        <?php echo $view['form']->render($form['title']) ?>

.. note::

    フィールドのテンプレートは、後で学習するようにフィールドのクラス名を元にして選択されています。

``label`` ヘルパーは、フィールドに関連付けられた ``<label>`` タグをレンダリングします。

.. configuration-block::

    .. code-block:: jinja

        {{ form_label(form.title) }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['title']) ?>

デフォルトでは、Symfony2 はフィールド名を「人間が読めるように」しますが、独自のラベルをつけることもできます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_label(form.title, 'Give me a title') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['title'], 'Give me a title') ?>

.. note::

    Symfony2 は自動的に全てのラベルとエラーメッセージを国際化します。

``errors`` ヘルパーはフィールドのエラーをレンダリングします。

.. configuration-block::

    .. code-block:: jinja

        {{ form_errors(form.title) }}

    .. code-block:: html+php

        <?php echo $view['form']->errors($form['title']) ?>

HTML の表現を定義する
---------------------

ヘルパーは HTML をレンダリングするために、テンプレートに依存しています。デフォルトで Symfony2 は、全てのビルトインフィールドに対してテンプレートが関連付けられています。

Twig テンプレートでは、それぞれのヘルパーは1つのテンプレートブロックに関連付けられています。例えば ``form_errors`` 関数は  ``errors`` ブロックに関連づいています。ビルトインフィールドは以下のように書かれています。

.. code-block:: html+jinja

    {# TwigBundle::form.html.twig #}

    {% block errors %}
        {% if errors %}
        <ul>
            {% for error in errors %}
                <li>{% trans error.0 with error.1 from validators %}</li>
            {% endfor %}
        </ul>
        {% endif %}
    {% endblock errors %}

PHP テンプレートではそれとは異なり、それぞれのヘルパーは1つの PHP テンプレートに関連づいています。 ``errors()`` ヘルパーは、以下のように ``errors.php`` テンプレートに関連づきます。

.. code-block:: html+php

    {# FrameworkBundle:Form:errors.php #}

    <?php if ($errors): ?>
        <ul>
            <?php foreach ($errors as $error): ?>
                <li><?php echo $view['translator']->trans($error[0], $error[1], 'validators') ?></li>
            <?php endforeach; ?>
        </ul>
    <?php endif; ?>

以下はヘルパーとそれに関連付けられたブロックやテンプレートの一覧です。

========== ================== ==================
ヘルパー   Twig ブロック      PHP テンプレート名
========== ================== ==================
``errors`` ``errors``         ``FrameworkBundle:Form:errors.php``
``hidden`` ``hidden``         ``FrameworkBundle:Form:hidden.php``
``label``  ``label``          ``FrameworkBundle:Form:label.php``
``render`` 下記参照           下記参照
========== ================== ==================

``render`` ヘルパーは、レンダリングするテンプレートをフィールドのクラス名をアンダースコアで区切ったものを元にして選ぶところが、他と少し異なります。例えば、 ``TextareaField`` インスタンスをレンダリングする際には、 ``textarea_field`` ブロックまたは ``textarea_field.php`` テンプレートを探します。

.. configuration-block::

    .. code-block:: html+jinja

        {# TwigBundle::form.html.twig #}

        {% block textarea_field %}
            <textarea {% display field_attributes %}>{{ field.displayedData }}</textarea>
        {% endblock textarea_field %}

    .. code-block:: html+php

        <!-- FrameworkBundle:Form:textarea_field.php -->
        <textarea id="<?php echo $field->getId() ?>" name="<?php echo $field->getName() ?>" <?php if ($field->isDisabled()): ?>disabled="disabled"<?php endif ?>>
            <?php echo $view->escape($field->getDisplayedData()) ?>
        </textarea>

ブロックやテンプレートが存在しない場合、メソッドはフィールドの継承元クラスのブロックやテンプレートを探します。表現が継承元クラスと同じになるよう、デフォルトの ``collection_field`` ブロックが存在しないのはこのためです。

フィールドの表現をカスタマイズする
----------------------------------

フィールドをカスタマイズする一番簡単な方法は、 ``render`` ヘルパーへの引数としてカスタムHTML属性を渡してやることです。

.. configuration-block::

    .. code-block:: jinja

        {{ form_field(form.title, { 'class': 'important' }) }}

    .. code-block:: html+php

        <?php echo $view['form']->render($form['title'], array(
            'class' => 'important'
        )) ?>

``ChoiceField`` のようないくつかのフィールドは、フィールドの表現をカスタマイズするためのパラメータを受け取ることができます。これらのパラメータは2番目以降の引数として渡せます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_field(form.country, {}, { 'separator': ' -- Other countries -- ' }) }}

    .. code-block:: html+php

        <?php echo $view['form']->render($form['country'], array(), array(
            'separator' => ' -- Other countries -- '
        )) ?>

全てのヘルパーは、ヘルパーの HTML 出力を完全に変えられるように、最後の引数としてテンプレートネームを受け取ることができます。

.. configuration-block::

    .. code-block:: jinja

        {{ form_field(form.title, {}, {}, 'HelloBundle::form.html.twig') }}

    .. code-block:: html+php

        <?php echo $view['form']->render($form['title'], array(), array(),
            'HelloBundle:Form:text_field.php'
        ) ?>

フォームのテーミング (Twig のみ)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

最後の例として、 ``HelloBundle::form.html.twig`` という、オーバーライドしたいフィールドの HTML 表現を定義するブロックを含んだ普通の Twig テンプレートを挙げます。

.. code-block:: html+jinja

    {# HelloBundle/Resources/views/form.html.twig #}

    {% block textarea_field %}
        <div class="textarea_field">
            <textarea {% display field_attributes %}>{{ field.displayedData }}</textarea>
        </div>
    {% endblock textarea_field %}

この例では、 ``textarea_field`` が再定義されています。デフォルトの表現を変える代わりに、Twig ネイティブの継承機能を使ってデフォルトのブロックを拡張することもできます。

.. code-block:: html+jinja

    {# HelloBundle/Resources/views/form.html.twig #}

    {% extends 'TwigBundle::form.html.twig' %}

    {% block date_field %}
        <div class="important_date_field">
            {{ parent() }}
        </div>
    {% endblock date_field %}

与えられたフォームの全てのフィールドをカスタマイズしたい時は、 ``form_theme`` タグを使いましょう。

.. code-block:: jinja

    {% form_theme form 'HelloBundle::form.html.twig' %}

この呼び出しの後、 ``form`` 上で ``form_field`` 関数を呼び出す時は常に、Symfony2 はデフォルトの表現に戻る前にテンプレート内の表現を探します。

フィールドブロックが幾つかのテンプレート内で定義されている場合、順序づけされた配列として追加してください。

.. code-block:: jinja

    {% form_theme form ['HelloBundle::form.html.twig', 'HelloBundle::form.html.twig', 'HelloBundle::hello_form.html.twig'] %}

フォーム全体 (上のように) あるいはフィールドグループに対してテーマが加えられます。

.. code-block:: jinja

    {% form_theme form.user 'HelloBundle::form.html.twig' %}

最終的に、アプリケーションのすべてのフォームの表現をカスタマイズすることは、コンフィギュレーションからも可能です。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        twig:
            form:
                resources: [BlogBundle::form.html.twig, TwigBundle::form.html.twig]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <twig:config>
            <twig:form>
                <twig:resource>BlogBundle::form.html.twig</twig:resource>
                <twig:resource>TwigBundle::form.html.twig</twig:resource>
            </twig:form>
        </twig:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('twig', array('form' => array(
            'resources' => array('BlogBundle::form.html.twig', 'TwigBundle::form.html.twig),
        )));

.. sidebar:: _self

    フォーム関数やタグがテンプレート名を引数として取る場合はいつでも、\ ``_self`` を代わりに使用することができます。また、そのテンプレートの中でカスタマイズを直接定義することも可能です。

    .. code-block:: html+jinja

        {% form_theme form _self %}

        {% block textarea_field %}
            ...
        {% endblock %}

        {{ form_field(form.description, {}, {}, _self) }}

試作
----

フォームの試作を行う時は、全てのフィールドを手動でレンダリングする代わりに、 ``render`` ヘルパーをフォーム上で使用できます。

.. configuration-block::

    .. code-block:: html+jinja

        <form action="#" {{ form_enctype(form) }} method="post">
            {{ form_field(form) }}
            <input type="submit" />
        </form>

    .. code-block:: html+php

        <form action="#" <?php echo $view['form']->enctype($form) ?> method="post">
            <?php echo $view['form']->render($form) ?>

            <input type="submit" />
        </form>

``Form`` クラスに対してブロックやテンプレートが定義されていないことから、継承元クラスの1つである ``FieldGroup`` が代わりに使用されます。

.. configuration-block::

    .. code-block:: html+jinja

        {# TwigBundle::form.html.twig #}

        {% block field_group %}
            {{ form_errors(field) }}
            {% for child in field %}
                {% if not child.ishidden %}
                    <div>
                        {{ form_label(child) }}
                        {{ form_errors(child) }}
                        {{ form_field(child) }}
                    </div>
                {% endif %}
            {% endfor %}
            {{ form_hidden(field) }}
        {% endblock field_group %}

    .. code-block:: html+php

        <!-- FrameworkBundle:Form:group/table/field_group.php -->

        <?php echo $view['form']->errors($field) ?>

        <div>
            <?php foreach ($field->getVisibleFields() as $child): ?>
                <div>
                    <?php echo $view['form']->label($child) ?>
                    <?php echo $view['form']->errors($child) ?>
                    <?php echo $view['form']->render($child) ?>
                </div>
            <?php endforeach; ?>
        </div>

        <?php echo $view['form']->hidden($field) ?>

.. caution::

    The ``render`` method is not very flexible and should only be used to
    build prototypes.
    ``render`` メソッドはそれほど柔軟性があるわけではないので、施策の際にのみ使用するのがよいでしょう。
