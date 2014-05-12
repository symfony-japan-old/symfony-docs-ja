label_attr
~~~~~~~~~~

**データ型**: ``array`` **デフォルト値**: ``array()``

フィールドのラベルを表示する際、 ``<label>`` 要素に html 属性を設定します。それは HTML 属性をキーとした連想配列です。この属性は直接テンプレートの中に設定することも出来ます。:

.. configuration-block::

    .. code-block:: jinja

        {{ form_label(form.name, 'あなたのお名前', {'label_attr': {'class': 'CUSTOM_LABEL_CLASS'}}) }}

    .. code-block:: php

        echo $view['form']->label(
            $form['name'],
            'あなたのお名前',
            array('label_attr' => array('class' => 'CUSTOM_LABEL_CLASS'))
        );

.. 2014/05/10 yositani2002 82c1db5644d1373f000e8e8dc1a3624b010faeef
