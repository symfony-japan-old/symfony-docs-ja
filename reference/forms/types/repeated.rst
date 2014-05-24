.. index::
   single: Forms; Fields; repeated

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/05/24

repeated フィールドタイプ
=========================

これは特別なフィールド "グループ" です。値が一致しなくてはいけない（または、検証エラーがスローされる）二つの同一のフィールドです。
ユーザーに検証のためパスワードや電子メールを繰り返し求めるのが最も一般的な使用法です。

+------------------+------------------------------------------------------------------------+
| 対応するデータ型 | ``text`` フィールドがデフォルト。 `type`_ オプションを参照             |
+------------------+------------------------------------------------------------------------+
| オプション       | - `type`_                                                              |
|                  | - `options`_                                                           |
|                  | - `first_options`_                                                     |
|                  | - `second_options`_                                                    |
|                  | - `first_name`_                                                        |
|                  | - `second_name`_                                                       |
+------------------+------------------------------------------------------------------------+
| 上書きされた     | - `error_bubbling`_                                                    |
| オプション       |                                                                        |
+------------------+------------------------------------------------------------------------+
| 継承された       | - `data`_                                                              |
| オプション       | - `invalid_message`_                                                   |
|                  | - `invalid_message_parameters`_                                        |
|                  | - `mapped`_                                                            |
|                  | - `error_mapping`_                                                     |
+------------------+------------------------------------------------------------------------+
| 親タイプ         | :doc:`form </reference/forms/types/form>`                              |
+------------------+------------------------------------------------------------------------+
| クラス           | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RepeatedType` |
+------------------+------------------------------------------------------------------------+

使用例
------

.. code-block:: php

    $builder->add('password', 'repeated', array(
        'type' => 'password',
        'invalid_message' => 'The password fields must match.',
        'options' => array('attr' => array('class' => 'password-field')),
        'required' => true,
        'first_options'  => array('label' => 'Password'),
        'second_options' => array('label' => 'Repeat Password'),
    ));

フォーム送信に成功したとき、 "password" フィールドの両方に入力された値は "password" キーのデータになります。
つまり、実際に2つのフィールドが描画されていても、フォームからのエンドデータはあなたが必要とする単一の値（通常は文字列）です。

最も重要なオプションは、\ ``type`` です。それは、任意のフィールドタイプを設定でき、2つの対応するフィールドの実際の型を決定します。
``options`` オプションは、それらの個々のフィールドにそれぞれに渡され、 - この例では - ``password`` タイプがサポートする任意のオプションをこの配列に渡すことができます。

Rendering
~~~~~~~~~

繰り返されるフィールドタイプは、実際には二つのフィールドを、すべてを一度に、または個別にレンダリングすることができます。
一度にすべてをレンダリングするには、以下のように使用します。:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.password) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']) ?>

個別に各フィールドをレンダリングするためには、このように使用します。:

.. configuration-block::

    .. code-block:: jinja

        {# .first and .second may vary in your use - see the note below #}
        {{ form_row(form.password.first) }}
        {{ form_row(form.password.second) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['password']['first']) ?>
        <?php echo $view['form']->row($form['password']['second']) ?>

.. note::

    ``first`` と ``second`` は二つのサブフィールドのデフォルトの名前です。
    しかし、これらの名前は `first_name`_ と `second_name`_ オプションを介して制御することができます。
    これらのオプションを設定している場合は、\ ``first`` と ``second`` をレンダリングするとき代わりにそれらの値を使用します。

Validation
~~~~~~~~~~

``repeated`` フィールドの主な特徴の一つは（設定するために何もする必要がない）内部検証です。2つのフィールドが一致する値を持つように強制します。
2つのフィールドが一致しない場合、ユーザにエラーが表示されます。

``invalid_message`` は2つのフィールドが一致しないときに表示されるエラーをカスタマイズするために使用されます。

フィールドオプション
--------------------

type
~~~~

**データ型**: ``string`` **デフォルト**: ``text``

その2つのフィールドはこのフィールドタイプになります。たとえば、\ ``password`` タイプを渡すと、二つの ``password`` フィールドをレンダリングします。

options
~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

このオプションの配列は二つのフィールドにそれぞれに渡されます。換言すれば、これらは、個々のフィールドタイプをカスタマイズするオプションがあります。
たとえば、\ ``type`` オプションに ``password`` を設定したとき、この配列には ``always_empty`` または ``required`` が含まれるでしょう。
どちらのオプションも ``password`` フィールドタイプでサポートされています。

first_options
~~~~~~~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

第一フィールド *だけ* に渡されるべき追加オプション（ `options` にマージされます）です。
これはラベルをカスタマイズする場合に特に便利です。::

    $builder->add('password', 'repeated', array(
        'first_options'  => array('label' => 'Password'),
        'second_options' => array('label' => 'Repeat Password'),
    ));

second_options
~~~~~~~~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

第二フィールド *だけ* に渡されるべき追加オプション（ `options` にマージされます）です。
これはラベルをカスタマイズする場合に特に便利です。（参照 `first_options`_ ）

first_name
~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``first``

これは、第一フィールドのために使用される実際のフィールド名です。
大部分には無意味ですが、両方のフィールドに入力された実際のデータは、\ ``repeated`` フィールド自体（例えば``password``）に
割り当てられるそのキーで利用できるようになります。
しかし、ラベルを指定しない場合はこのフィールド名はラベルを「推測」するために使用されます。

second_name
~~~~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``second``

``first_name`` と同様ですが、第二フィールドのためのものです。

上書きされたオプション
----------------------

error_bubbling
~~~~~~~~~~~~~~

**デフォルト**: ``false``

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. 2014/05/24 yositani2002 d49d12eaf265a5d6d32ac660c62f385d57261475
